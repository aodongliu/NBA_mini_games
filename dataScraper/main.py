from database.db_manager import DatabaseManager
from scrapers.nba_stats_scraper import NBAStatsScraper
from scrapers.image_scraper import ImageScraper
import os

def main():
    # Initialize database
    db_manager = DatabaseManager()
    session = db_manager.get_session()
    
    # Initialize scrapers
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    image_scraper = ImageScraper(base_dir)
    stats_scraper = NBAStatsScraper(session, image_scraper)
    
    try:
        # Initialize teams first
        stats_scraper.initialize_teams()
        
        # Then scrape players
        stats_scraper.scrape_all_players()
        
        # Commit changes
        session.commit()
        print("Data collection complete!")
        
    except Exception as e:
        session.rollback()
        print(f"Error during data collection: {str(e)}")
        
    finally:
        session.close()

if __name__ == "__main__":
    main() 