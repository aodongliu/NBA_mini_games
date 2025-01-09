"""
Basic verification tests for NBA data.
Checks that required data and images exist.
"""

import os
from pathlib import Path
from database.db_manager import DatabaseManager
from database.schema import Team, Player

def test_data():
    """Verify basic data requirements."""
    db = DatabaseManager()
    session = db.get_session()
    
    try:
        # Check teams exist
        teams = session.query(Team).all()
        if len(teams) != 30:
            print(f"Warning: Expected 30 teams, found {len(teams)}")
        
        # Check players and their images
        players = session.query(Player).all()
        if not players:
            print("Warning: No players found in database")
            return
            
        for player in players:
            # Verify image exists
            if not os.path.exists(player.headshot_path):
                print(f"Missing image for {player.first_name} {player.last_name}")
            
            # Verify has team
            if not player.team:
                print(f"No team found for {player.first_name} {player.last_name}")
                
        print("Basic data verification complete")
        
    finally:
        session.close()

if __name__ == "__main__":
    test_data() 