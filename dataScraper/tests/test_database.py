"""
Database verification and testing utilities.
Provides comprehensive checks for database integrity, relationships,
and data consistency.
"""

import logging
from pathlib import Path
from typing import Dict, List, Optional
from sqlalchemy.sql import select
from database.db_manager import DatabaseManager
from database.schema import Team, Player, PlayerStats
from sqlalchemy import inspect

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class DatabaseTester:
    """
    Test suite for database verification and validation.
    
    Provides methods to verify:
    - Database structure
    - Data integrity
    - Relationships between tables
    - Data consistency
    """
    
    def __init__(self, db_path: Optional[str] = None):
        """Initialize database tester with optional custom path."""
        self.db_manager = DatabaseManager(db_path)
        self.session = self.db_manager.get_session()

    def run_all_tests(self) -> bool:
        """
        Run all database verification tests.
        
        Returns:
            bool: True if all tests pass, False otherwise
        """
        try:
            tests = [
                self.verify_tables,
                self.verify_team_data,
                self.verify_player_data,
                self.verify_stats_data,
                self.verify_relationships
            ]
            
            all_passed = True
            for test in tests:
                if not test():
                    all_passed = False
            
            return all_passed
        finally:
            self.session.close()

    def verify_tables(self) -> bool:
        """Verify all required tables exist with correct schema."""
        try:
            required_tables = ['teams', 'players', 'player_stats']
            inspector = inspect(self.db_manager.engine)
            
            for table in required_tables:
                if not inspector.has_table(table):
                    logger.error(f"Missing required table: {table}")
                    return False
            logger.info("All required tables present")
            return True
        except Exception as e:
            logger.error(f"Table verification failed: {str(e)}")
            return False

    def verify_team_data(self) -> bool:
        """Verify team data integrity and completeness."""
        try:
            teams = self.session.query(Team).all()
            
            # Check we have all 30 NBA teams
            if len(teams) != 30:
                logger.error(f"Expected 30 teams, found {len(teams)}")
                return False
            
            # Verify each team has required data
            for team in teams:
                if not all([
                    team.id,
                    team.name,
                    team.abbreviation,
                    team.conference in ['Eastern', 'Western'],
                    team.division,
                    team.logo_path
                ]):
                    logger.error(f"Incomplete data for team: {team.name}")
                    return False
                
                # Verify logo file exists
                if not Path(team.logo_path).exists():
                    logger.error(f"Missing logo for team: {team.name}")
                    return False
            
            logger.info("Team data verification passed")
            return True
        except Exception as e:
            logger.error(f"Team verification failed: {str(e)}")
            return False

    def verify_player_data(self) -> bool:
        """Verify player data integrity and relationships."""
        try:
            players = self.session.query(Player).all()
            
            if not players:
                logger.error("No players found in database")
                return False
            
            for player in players:
                # Verify required fields
                if not all([
                    player.id,
                    player.first_name,
                    player.last_name,
                    player.team_id,
                    player.headshot_path
                ]):
                    logger.error(f"Incomplete data for player: {player.full_name}")
                    return False
                
                # Verify headshot exists
                if not Path(player.headshot_path).exists():
                    logger.error(f"Missing headshot for player: {player.full_name}")
                    return False
                
                # Verify team relationship
                if not player.team:
                    logger.error(f"Player {player.full_name} has no team")
                    return False
            
            logger.info("Player data verification passed")
            return True
        except Exception as e:
            logger.error(f"Player verification failed: {str(e)}")
            return False

    def verify_stats_data(self) -> bool:
        """Verify player statistics data integrity."""
        try:
            stats = self.session.query(PlayerStats).all()
            
            for stat in stats:
                # Verify player relationship
                if not stat.player:
                    logger.error(f"Stats record {stat.id} has no player")
                    return False
                
                # Verify numeric data is valid
                if not all([
                    isinstance(stat.games_played, int),
                    isinstance(stat.points_per_game, float),
                    0 <= stat.field_goal_percentage <= 1,
                    stat.last_updated is not None
                ]):
                    logger.error(f"Invalid stats data for player: {stat.player.full_name}")
                    return False
            
            logger.info("Stats data verification passed")
            return True
        except Exception as e:
            logger.error(f"Stats verification failed: {str(e)}")
            return False

    def verify_relationships(self) -> bool:
        """Verify all database relationships are valid."""
        try:
            # Check team-player relationships
            teams = self.session.query(Team).all()
            for team in teams:
                if not isinstance(team.players, list):
                    logger.error(f"Invalid players relationship for team: {team.name}")
                    return False
            
            # Check player-stats relationships
            players = self.session.query(Player).all()
            for player in players:
                if not player.stats:
                    logger.error(f"Missing stats for player: {player.full_name}")
                    return False
            
            logger.info("Relationship verification passed")
            return True
        except Exception as e:
            logger.error(f"Relationship verification failed: {str(e)}")
            return False

def main():
    """Run all database tests."""
    tester = DatabaseTester()
    if tester.run_all_tests():
        logger.info("All database tests passed!")
    else:
        logger.error("Some database tests failed")

if __name__ == "__main__":
    main() 