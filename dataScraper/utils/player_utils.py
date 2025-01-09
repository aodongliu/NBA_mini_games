"""
Utility functions for handling NBA player data parsing and validation.
This module provides standardized methods for working with player names
and other player-related data across the application.
"""

def parse_player_name(name_data):
    """
    Parse a player's name from NBA API data format into first and last name.
    
    The NBA API returns player names in two formats:
    - name_data[1]: "LastName, FirstName" (e.g., "James, LeBron")
    - name_data[2]: "FirstName LastName" (e.g., "LeBron James")
    
    Args:
        name_data (list): Raw player data from NBA API containing name fields
        
    Returns:
        tuple: (first_name, last_name)
        
    Example:
        >>> data = [123, "James, LeBron", "LeBron James"]
        >>> parse_player_name(data)
        ('LeBron', 'James')
    """
    if ',' in name_data[1]:
        last_name, first_name = name_data[1].split(',', 1)
        first_name = first_name.strip().split()[0]  # Take first word after comma
        last_name = last_name.strip()  # Take everything before comma
    else:
        # Fallback if no comma: split the second field
        name_parts = name_data[2].split()
        first_name = name_parts[0]
        last_name = name_parts[-1]
    
    return first_name, last_name

def validate_player_name(first_name, last_name):
    """
    Validate player name components for database storage.
    
    Args:
        first_name (str): Player's first name
        last_name (str): Player's last name
        
    Returns:
        bool: True if valid, False otherwise
        
    Raises:
        ValueError: If either name component is invalid
    """
    if not first_name or not last_name:
        raise ValueError("Both first and last name must be provided")
        
    if len(first_name) < 2 or len(last_name) < 2:
        raise ValueError("Names must be at least 2 characters long")
        
    return True

def generate_player_filename(player_id, first_name, last_name):
    """
    Generate a standardized filename for player images.
    
    Args:
        player_id (int): NBA player ID
        first_name (str): Player's first name
        last_name (str): Player's last name
        
    Returns:
        str: Formatted filename (e.g., "2544_lebron_james")
    """
    name = f"{first_name}_{last_name}".lower()
    return f"{player_id}_{name}" 