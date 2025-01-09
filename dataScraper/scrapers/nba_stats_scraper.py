"""
NBA Stats API scraper for player and team data.
Handles fetching and processing player statistics, team information,
and manages data persistence to the database.
"""

import requests
import time
from datetime import datetime
import logging
from typing import Optional, Dict, Any, List
from database.schema import Team, Player, PlayerStats
from utils.player_utils import parse_player_name

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class NBAStatsScraper:
    """
    Scraper for NBA.com stats API.
    
    Handles fetching player data, team information, and statistics
    from the NBA's official stats API.
    
    Attributes:
        session: SQLAlchemy session for database operations
        image_scraper: ImageScraper instance for downloading player/team images
        current_season (str): Current NBA season in format "YYYY-YY"
    """
    
    def __init__(self, session, image_scraper):
        """
        Initialize the NBA stats scraper.
        
        Args:
            session: SQLAlchemy session for database operations
            image_scraper: ImageScraper instance for handling images
        """
        self.session = session
        self.image_scraper = image_scraper
        self.current_season = "2024-25"
        
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36',
            'Referer': 'https://www.nba.com',
            'Accept': 'application/json'
        }

    def get_player_stats(self, player_id: int) -> Optional[PlayerStats]:
        """
        Fetch current season statistics for a player.
        Returns default stats if player has no data yet.
        """
        url = "https://stats.nba.com/stats/playerprofilev2"
        params = {
            'PlayerID': player_id,
            'PerMode': 'PerGame',
            'LeagueID': '00'
        }
        
        try:
            response = requests.get(url, headers=self.headers, params=params)
            response.raise_for_status()
            data = response.json()
            
            # Check if player has any stats
            if not data['resultSets'][0]['rowSet']:
                logger.info(f"No stats available for player {player_id}, using defaults")
                return PlayerStats(
                    player_id=player_id,
                    season=self.current_season,
                    games_played=0,
                    games_started=0,
                    minutes_per_game=0.0,
                    field_goal_percentage=0.0,
                    three_point_percentage=0.0,
                    free_throw_percentage=0.0,
                    rebounds_per_game=0.0,
                    assists_per_game=0.0,
                    steals_per_game=0.0,
                    blocks_per_game=0.0,
                    points_per_game=0.0,
                    last_updated=datetime.now()
                )
            
            # Get current season stats from the last row
            season_stats = data['resultSets'][0]['rowSet'][-1]
            headers = data['resultSets'][0]['headers']
            stats_dict = dict(zip(headers, season_stats))
            
            return PlayerStats(
                player_id=player_id,
                season=self.current_season,
                games_played=int(stats_dict.get('GP', 0)),
                games_started=int(stats_dict.get('GS', 0)),
                minutes_per_game=float(stats_dict.get('MIN', 0.0)),
                field_goal_percentage=float(stats_dict.get('FG_PCT', 0.0)),
                three_point_percentage=float(stats_dict.get('FG3_PCT', 0.0)),
                free_throw_percentage=float(stats_dict.get('FT_PCT', 0.0)),
                rebounds_per_game=float(stats_dict.get('REB', 0.0)),
                assists_per_game=float(stats_dict.get('AST', 0.0)),
                steals_per_game=float(stats_dict.get('STL', 0.0)),
                blocks_per_game=float(stats_dict.get('BLK', 0.0)),
                points_per_game=float(stats_dict.get('PTS', 0.0)),
                last_updated=datetime.now()
            )
            
        except Exception as e:
            logger.error(f"Error fetching stats for player {player_id}: {str(e)}")
            # Return default stats on error
            return PlayerStats(
                player_id=player_id,
                season=self.current_season,
                games_played=0,
                games_started=0,
                minutes_per_game=0.0,
                field_goal_percentage=0.0,
                three_point_percentage=0.0,
                free_throw_percentage=0.0,
                rebounds_per_game=0.0,
                assists_per_game=0.0,
                steals_per_game=0.0,
                blocks_per_game=0.0,
                points_per_game=0.0,
                last_updated=datetime.now()
            )

    def scrape_all_players(self) -> bool:
        """
        Fetch and store data for all active NBA players.
        """
        url = "https://stats.nba.com/stats/commonallplayers"
        params = {
            'LeagueID': '00',
            'Season': self.current_season,
            'IsOnlyCurrentSeason': '1'
        }
        
        try:
            response = requests.get(url, headers=self.headers, params=params)
            response.raise_for_status()
            data = response.json()
            
            # Initialize teams first
            logger.info("Initializing teams...")
            self.initialize_teams()
            
            # Process each player
            for player_data in data['resultSets'][0]['rowSet']:
                try:
                    player_id = player_data[0]
                    team_id = str(player_data[8])
                    is_active = player_data[3]
                    
                    if is_active == 1:
                        first_name, last_name = parse_player_name(player_data)
                        logger.info(f"Processing player: {first_name} {last_name}")
                        
                        # Download player headshot
                        player_name = f"{first_name}_{last_name}".lower()
                        headshot_path = self.image_scraper.download_player_headshot(
                            player_id, 
                            player_name
                        )
                        
                        # Create player record
                        player = Player(
                            id=player_id,
                            first_name=first_name,
                            last_name=last_name,
                            team_id=team_id,
                            headshot_path=headshot_path
                        )
                        
                        # Get stats with exponential backoff
                        wait_time = 1
                        max_retries = 3
                        for attempt in range(max_retries):
                            try:
                                if stats := self.get_player_stats(player_id):
                                    player.stats = stats
                                    break
                            except Exception as e:
                                if attempt == max_retries - 1:
                                    logger.error(f"Failed to get stats for {first_name} {last_name} after {max_retries} attempts")
                                else:
                                    wait_time *= 2  # Exponential backoff
                                    logger.info(f"Retrying in {wait_time} seconds...")
                                    time.sleep(wait_time)
                        
                        self.session.merge(player)
                        self.session.flush()
                        time.sleep(1.5)  # Increased base delay between players
                        
                except Exception as e:
                    logger.error(f"Error processing player: {str(e)}")
                    continue
            
            self.session.commit()
            return True
            
        except Exception as e:
            logger.error(f"Error scraping players: {str(e)}")
            self.session.rollback()
            return False

    def initialize_teams(self) -> None:
        """Initialize or update all NBA teams in the database."""
        teams_data = {
            '1610612737': {'name': 'Atlanta Hawks', 'abbreviation': 'ATL', 'conference': 'Eastern', 'division': 'Southeast'},
            '1610612738': {'name': 'Boston Celtics', 'abbreviation': 'BOS', 'conference': 'Eastern', 'division': 'Atlantic'},
            '1610612751': {'name': 'Brooklyn Nets', 'abbreviation': 'BKN', 'conference': 'Eastern', 'division': 'Atlantic'},
            '1610612766': {'name': 'Charlotte Hornets', 'abbreviation': 'CHA', 'conference': 'Eastern', 'division': 'Southeast'},
            '1610612741': {'name': 'Chicago Bulls', 'abbreviation': 'CHI', 'conference': 'Eastern', 'division': 'Central'},
            '1610612739': {'name': 'Cleveland Cavaliers', 'abbreviation': 'CLE', 'conference': 'Eastern', 'division': 'Central'},
            '1610612742': {'name': 'Dallas Mavericks', 'abbreviation': 'DAL', 'conference': 'Western', 'division': 'Southwest'},
            '1610612743': {'name': 'Denver Nuggets', 'abbreviation': 'DEN', 'conference': 'Western', 'division': 'Northwest'},
            '1610612765': {'name': 'Detroit Pistons', 'abbreviation': 'DET', 'conference': 'Eastern', 'division': 'Central'},
            '1610612744': {'name': 'Golden State Warriors', 'abbreviation': 'GSW', 'conference': 'Western', 'division': 'Pacific'},
            '1610612745': {'name': 'Houston Rockets', 'abbreviation': 'HOU', 'conference': 'Western', 'division': 'Southwest'},
            '1610612754': {'name': 'Indiana Pacers', 'abbreviation': 'IND', 'conference': 'Eastern', 'division': 'Central'},
            '1610612746': {'name': 'LA Clippers', 'abbreviation': 'LAC', 'conference': 'Western', 'division': 'Pacific'},
            '1610612747': {'name': 'Los Angeles Lakers', 'abbreviation': 'LAL', 'conference': 'Western', 'division': 'Pacific'},
            '1610612763': {'name': 'Memphis Grizzlies', 'abbreviation': 'MEM', 'conference': 'Western', 'division': 'Southwest'},
            '1610612748': {'name': 'Miami Heat', 'abbreviation': 'MIA', 'conference': 'Eastern', 'division': 'Southeast'},
            '1610612749': {'name': 'Milwaukee Bucks', 'abbreviation': 'MIL', 'conference': 'Eastern', 'division': 'Central'},
            '1610612750': {'name': 'Minnesota Timberwolves', 'abbreviation': 'MIN', 'conference': 'Western', 'division': 'Northwest'},
            '1610612740': {'name': 'New Orleans Pelicans', 'abbreviation': 'NOP', 'conference': 'Western', 'division': 'Southwest'},
            '1610612752': {'name': 'New York Knicks', 'abbreviation': 'NYK', 'conference': 'Eastern', 'division': 'Atlantic'},
            '1610612760': {'name': 'Oklahoma City Thunder', 'abbreviation': 'OKC', 'conference': 'Western', 'division': 'Northwest'},
            '1610612753': {'name': 'Orlando Magic', 'abbreviation': 'ORL', 'conference': 'Eastern', 'division': 'Southeast'},
            '1610612755': {'name': 'Philadelphia 76ers', 'abbreviation': 'PHI', 'conference': 'Eastern', 'division': 'Atlantic'},
            '1610612756': {'name': 'Phoenix Suns', 'abbreviation': 'PHX', 'conference': 'Western', 'division': 'Pacific'},
            '1610612757': {'name': 'Portland Trail Blazers', 'abbreviation': 'POR', 'conference': 'Western', 'division': 'Northwest'},
            '1610612758': {'name': 'Sacramento Kings', 'abbreviation': 'SAC', 'conference': 'Western', 'division': 'Pacific'},
            '1610612759': {'name': 'San Antonio Spurs', 'abbreviation': 'SAS', 'conference': 'Western', 'division': 'Southwest'},
            '1610612761': {'name': 'Toronto Raptors', 'abbreviation': 'TOR', 'conference': 'Eastern', 'division': 'Atlantic'},
            '1610612762': {'name': 'Utah Jazz', 'abbreviation': 'UTA', 'conference': 'Western', 'division': 'Northwest'},
            '1610612764': {'name': 'Washington Wizards', 'abbreviation': 'WAS', 'conference': 'Eastern', 'division': 'Southeast'}
        }
        
        for team_id, team_info in teams_data.items():
            try:
                # Download team logo
                logo_path = self.image_scraper.download_team_logo(
                    team_id,
                    team_info['name']
                )
                
                team = Team(
                    id=team_id,
                    name=team_info['name'],
                    abbreviation=team_info['abbreviation'],
                    conference=team_info['conference'],
                    division=team_info['division'],
                    logo_path=logo_path
                )
                self.session.merge(team)
                
            except Exception as e:
                logger.error(f"Error initializing team {team_info['name']}: {str(e)}")
                continue
        
        self.session.commit() 