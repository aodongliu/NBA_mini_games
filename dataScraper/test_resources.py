"""
Basic tests to verify NBA data resources and database content.
"""

import os
from pathlib import Path
from database.db_manager import DatabaseManager
from database.schema import Team, Player

def test_resources():
    """Test if required resources exist and database has valid data."""
    # Get project paths
    project_root = Path(__file__).parent.parent
    resources_dir = project_root / "resources"
    
    # 1. Check if required directories exist
    required_dirs = [
        resources_dir / "data",
        resources_dir / "player_images",
        resources_dir / "team_logos"
    ]
    
    for directory in required_dirs:
        if not directory.exists():
            print(f"Missing directory: {directory}")
            return False
    
    # 2. Check if database exists
    db_path = resources_dir / "data" / "nba_data.db"
    if not db_path.exists():
        print(f"Database not found at: {db_path}")
        return False
        
    # 3. Check database content
    db = DatabaseManager()
    session = db.get_session()
    
    try:
        # Check teams
        teams = session.query(Team).all()
        if len(teams) != 30:
            print(f"Wrong number of teams: {len(teams)} (expected 30)")
            return False
            
        # Check some team data
        for team in teams:
            if not all([team.name, team.abbreviation, team.conference]):
                print(f"Incomplete data for team: {team.name}")
                return False
            
            # Check team logo exists
            if not os.path.exists(team.logo_path):
                print(f"Missing logo for team: {team.name}")
                return False
        
        # Check players
        players = session.query(Player).all()
        if not players:
            print("No players found in database")
            return False
            
        # Check some player data
        for player in players:
            # Basic info exists
            if not all([player.first_name, player.last_name, player.team_id]):
                print(f"Incomplete data for player: {player.first_name} {player.last_name}")
                return False
                
            # Image exists
            if not os.path.exists(player.headshot_path):
                print(f"Missing image for: {player.first_name} {player.last_name}")
                return False
                
            # Has valid team
            if not player.team:
                print(f"No team found for: {player.first_name} {player.last_name}")
                return False
        
        print("All basic tests passed!")
        return True
        
    finally:
        session.close()

if __name__ == "__main__":
    test_resources() 