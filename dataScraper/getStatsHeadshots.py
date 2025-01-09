import requests
import json
import os
from bs4 import BeautifulSoup
import time
from datetime import datetime

class NBADataScraper:
    def __init__(self):
        self.base_url = "https://www.nba.com"
        self.stats_url = "https://www.nba.com/stats"
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
        }
        self.players_data = []
        
        # Create directories if they don't exist
        self.base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        self.resources_dir = os.path.join(self.base_dir, 'resources')
        self.headshots_dir = os.path.join(self.resources_dir, 'headshots')
        self.data_dir = os.path.join(self.resources_dir, 'data')
        
        for directory in [self.resources_dir, self.headshots_dir, self.data_dir]:
            if not os.path.exists(directory):
                os.makedirs(directory)

    def get_active_players(self):
        """
        Get current NBA players using the NBA stats API
        """
        url = "https://stats.nba.com/stats/commonallplayers"
        params = {
            'LeagueID': '00',
            'Season': '2023-24',
            'IsOnlyCurrentSeason': '1'
        }
        
        try:
            response = requests.get(url, headers=self.headers, params=params)
            data = response.json()
            
            for player in data['resultSets'][0]['rowSet']:
                player_id = player[0]
                name = f"{player[1]} {player[2]}"
                team_id = player[7]
                is_active = player[3]
                
                if is_active == 1:  # Only get active players
                    self.players_data.append({
                        'id': player_id,
                        'name': name,
                        'firstName': player[1],
                        'lastName': player[2],
                        'teamId': team_id,
                        'team': self.get_team_name(team_id),
                        'stats': self.get_player_stats(player_id)
                    })
                    
                    # Download player headshot
                    self.download_player_headshot(player_id, name)
                    time.sleep(1)  # Be nice to the server
                    
            return True
            
        except Exception as e:
            print(f"Error getting active players: {str(e)}")
            return False

    def get_team_name(self, team_id):
        """Map team ID to team name"""
        teams = {
            '1610612737': 'Atlanta Hawks',
            '1610612738': 'Boston Celtics',
            '1610612751': 'Brooklyn Nets',
            '1610612766': 'Charlotte Hornets',
            '1610612741': 'Chicago Bulls',
            '1610612739': 'Cleveland Cavaliers',
            '1610612742': 'Dallas Mavericks',
            '1610612743': 'Denver Nuggets',
            '1610612765': 'Detroit Pistons',
            '1610612744': 'Golden State Warriors',
            '1610612745': 'Houston Rockets',
            '1610612754': 'Indiana Pacers',
            '1610612746': 'LA Clippers',
            '1610612747': 'Los Angeles Lakers',
            '1610612763': 'Memphis Grizzlies',
            '1610612748': 'Miami Heat',
            '1610612749': 'Milwaukee Bucks',
            '1610612750': 'Minnesota Timberwolves',
            '1610612740': 'New Orleans Pelicans',
            '1610612752': 'New York Knicks',
            '1610612760': 'Oklahoma City Thunder',
            '1610612753': 'Orlando Magic',
            '1610612755': 'Philadelphia 76ers',
            '1610612756': 'Phoenix Suns',
            '1610612757': 'Portland Trail Blazers',
            '1610612758': 'Sacramento Kings',
            '1610612759': 'San Antonio Spurs',
            '1610612761': 'Toronto Raptors',
            '1610612762': 'Utah Jazz',
            '1610612764': 'Washington Wizards'
        }
        return teams.get(str(team_id), "Free Agent")

    def get_player_stats(self, player_id):
        """Get basic stats for a player"""
        url = f"https://stats.nba.com/stats/playerprofilev2"
        params = {
            'PlayerID': player_id,
            'PerMode': 'PerGame',
            'LeagueID': '00'
        }
        
        try:
            response = requests.get(url, headers=self.headers, params=params)
            data = response.json()
            
            # Get current season stats
            current_season_stats = data['resultSets'][0]['rowSet'][-1]
            return {
                'ppg': current_season_stats[3],
                'rpg': current_season_stats[5],
                'apg': current_season_stats[4],
                'games': current_season_stats[2]
            }
        except:
            return {
                'ppg': 0.0,
                'rpg': 0.0,
                'apg': 0.0,
                'games': 0
            }

    def download_player_headshot(self, player_id, player_name):
        """Download player headshot image"""
        url = f"https://ak-static.cms.nba.com/wp-content/uploads/headshots/nba/latest/260x190/{player_id}.png"
        
        try:
            response = requests.get(url, headers=self.headers)
            if response.status_code == 200:
                filename = f"{player_id}_{player_name.replace(' ', '_')}.png"
                filepath = os.path.join(self.headshots_dir, filename)
                
                with open(filepath, 'wb') as f:
                    f.write(response.content)
                return True
        except:
            print(f"Failed to download headshot for {player_name}")
            return False

    def save_data(self):
        """Save all player data to JSON file"""
        current_season = "2023-24"
        filename = f"players_data_{current_season}.json"
        filepath = os.path.join(self.data_dir, filename)
        
        with open(filepath, 'w') as f:
            json.dump({
                'last_updated': datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                'season': current_season,
                'players': self.players_data
            }, f, indent=2)

def main():
    scraper = NBADataScraper()
    if scraper.get_active_players():
        scraper.save_data()
        print("Data collection complete!")
    else:
        print("Failed to collect data")

if __name__ == "__main__":
    main()
