import os
import requests

class HeadshotScraper:
    def __init__(self, base_dir):
        self.headshots_dir = os.path.join(base_dir, 'resources', 'headshots')
        if not os.path.exists(self.headshots_dir):
            os.makedirs(self.headshots_dir)
        
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
        }

    def download_headshot(self, player_id, player_name):
        url = f"https://ak-static.cms.nba.com/wp-content/uploads/headshots/nba/latest/260x190/{player_id}.png"
        
        try:
            response = requests.get(url, headers=self.headers)
            if response.status_code == 200:
                filename = f"{player_id}_{player_name.replace(' ', '_')}.png"
                filepath = os.path.join(self.headshots_dir, filename)
                
                with open(filepath, 'wb') as f:
                    f.write(response.content)
                return filepath
        except Exception as e:
            print(f"Failed to download headshot for {player_name}: {str(e)}")
            return None 