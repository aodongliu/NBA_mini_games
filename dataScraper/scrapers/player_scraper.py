def parse_player_name(name):
    """Parse player name from 'LastName, FirstName FirstName LastName' format"""
    if ',' not in name:
        return name, name  # Return as is if not in expected format
        
    # Split at comma: "Barnes, Scottie Scottie Barnes" -> ["Barnes", "Scottie Scottie Barnes"]
    last_name, full_name = name.split(',', 1)
    last_name = last_name.strip()
    
    # Get first name from the full name part
    first_name = full_name.strip().split()[0]
    
    return first_name, last_name

def scrape_players(self):
    """Scrape player data and insert into database"""
    # ... existing code ...
    
    for player in player_data:
        first_name, last_name = parse_player_name(player['name'])
        
        player_info = {
            'id': player['id'],
            'first_name': first_name,
            'last_name': last_name,
            'team_id': player['team_id'],
            'headshot_path': headshot_path
        }
        # ... rest of the code ... 