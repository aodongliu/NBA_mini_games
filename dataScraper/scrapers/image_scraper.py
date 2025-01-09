"""
Image scraper for NBA player headshots and team logos.
Handles downloading, caching, and managing image resources for the application.
"""

import os
import requests
from pathlib import Path
import logging
from typing import Optional

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class ImageScraper:
    """
    Manages downloading and storing NBA player and team images.
    
    Attributes:
        base_dir (Path): Root directory of the project
        player_images_dir (Path): Directory for player headshots
        team_logos_dir (Path): Directory for team logos
    """
    
    def __init__(self, base_dir: str):
        """
        Initialize the image scraper with base directory paths.
        
        Args:
            base_dir (str): Root directory path for the project
        """
        self.base_dir = Path(base_dir)
        self.player_images_dir = self.base_dir / "resources" / "player_images"
        self.team_logos_dir = self.base_dir / "resources" / "team_logos"
        
        # Create directories if they don't exist
        self.player_images_dir.mkdir(parents=True, exist_ok=True)
        self.team_logos_dir.mkdir(parents=True, exist_ok=True)
        
        # Headers for NBA.com requests
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36',
            'Referer': 'https://www.nba.com'
        }

    def download_player_headshot(self, player_id: int, player_name: str) -> Optional[str]:
        """
        Download a player's headshot from NBA.com.
        
        Args:
            player_id (int): NBA player ID
            player_name (str): Player's name for filename
            
        Returns:
            Optional[str]: Path to saved image file, or None if download fails
            
        Example:
            >>> scraper.download_player_headshot(2544, "lebron_james")
            "resources/player_images/2544_lebron_james.png"
        """
        filename = f"{player_id}_{player_name}.png"
        save_path = self.player_images_dir / filename
        
        # Return existing file if it exists
        if save_path.exists():
            logger.info(f"Using cached headshot for {player_name}")
            return str(save_path)
            
        # NBA.com headshot URL format
        url = f"https://ak-static.cms.nba.com/wp-content/uploads/headshots/nba/latest/260x190/{player_id}.png"
        
        try:
            response = requests.get(url, headers=self.headers)
            response.raise_for_status()
            
            with open(save_path, 'wb') as f:
                f.write(response.content)
            logger.info(f"Downloaded headshot for {player_name}")
            return str(save_path)
            
        except Exception as e:
            logger.error(f"Failed to download headshot for {player_name}: {str(e)}")
            return None

    def download_team_logo(self, team_id: str, team_name: str) -> Optional[str]:
        """
        Download a team's logo from NBA.com.
        
        Args:
            team_id (str): NBA team ID
            team_name (str): Team name for filename
            
        Returns:
            Optional[str]: Path to saved logo file, or None if download fails
        """
        filename = f"{team_id}_{team_name.lower().replace(' ', '_')}.svg"
        save_path = self.team_logos_dir / filename
        
        if save_path.exists():
            logger.info(f"Using cached logo for {team_name}")
            return str(save_path)
            
        # NBA.com logo URL format
        url = f"https://cdn.nba.com/logos/nba/{team_id}/primary/L/logo.svg"
        
        try:
            response = requests.get(url, headers=self.headers)
            response.raise_for_status()
            
            with open(save_path, 'wb') as f:
                f.write(response.content)
            logger.info(f"Downloaded logo for {team_name}")
            return str(save_path)
            
        except Exception as e:
            logger.error(f"Failed to download logo for {team_name}: {str(e)}")
            return None

    def verify_image(self, image_path: str) -> bool:
        """
        Verify that an image file exists and is valid.
        
        Args:
            image_path (str): Path to image file
            
        Returns:
            bool: True if image is valid, False otherwise
        """
        try:
            path = Path(image_path)
            return path.exists() and path.stat().st_size > 0
        except Exception as e:
            logger.error(f"Error verifying image {image_path}: {str(e)}")
            return False 