"""
Database manager for NBA data application.
Handles database initialization, connection management, and session creation.
Provides a centralized interface for database operations.
"""

import os
from pathlib import Path
import logging
from typing import Optional
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, Session
from sqlalchemy.exc import SQLAlchemyError
from .schema import Base

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class DatabaseManager:
    """
    Manages SQLite database connections and sessions.
    
    Handles database initialization, schema creation, and provides
    session management for database operations.
    
    Attributes:
        db_path (Path): Path to SQLite database file
        engine: SQLAlchemy engine instance
        Session: SQLAlchemy sessionmaker instance
    """
    
    def __init__(self, db_path: Optional[str] = None):
        """
        Initialize database manager.
        
        Args:
            db_path: Optional path to database file. If not provided,
                    uses default path from project structure.
        """
        if db_path is None:
            # Default to project's resources/data directory
            base_dir = Path(__file__).parent.parent.parent
            db_path = str(base_dir / "resources" / "data" / "nba_data.db")
        
        self.db_path = Path(db_path)
        self.db_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Initialize database connection
        try:
            self.engine = create_engine(f"sqlite:///{self.db_path}")
            self.Session = sessionmaker(bind=self.engine)
            self._create_tables()
            logger.info(f"Database initialized at {self.db_path}")
        except SQLAlchemyError as e:
            logger.error(f"Database initialization failed: {str(e)}")
            raise

    def _create_tables(self) -> None:
        """Create database tables if they don't exist."""
        try:
            Base.metadata.create_all(self.engine)
            logger.info("Database tables created successfully")
        except SQLAlchemyError as e:
            logger.error(f"Failed to create tables: {str(e)}")
            raise

    def get_session(self) -> Session:
        """
        Get a new database session.
        
        Returns:
            Session: New SQLAlchemy session
            
        Note:
            Caller is responsible for closing the session
        """
        return self.Session()

    def verify_database(self) -> bool:
        """
        Verify database integrity and required tables.
        
        Returns:
            bool: True if database is valid, False otherwise
        """
        try:
            session = self.get_session()
            # Check if tables exist and have correct schema
            for table in Base.metadata.tables.values():
                if not self.engine.dialect.has_table(self.engine, table.name):
                    logger.error(f"Missing table: {table.name}")
                    return False
            session.close()
            return True
        except SQLAlchemyError as e:
            logger.error(f"Database verification failed: {str(e)}")
            return False

    def clear_database(self) -> bool:
        """
        Clear all data from the database.
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            Base.metadata.drop_all(self.engine)
            Base.metadata.create_all(self.engine)
            logger.info("Database cleared successfully")
            return True
        except SQLAlchemyError as e:
            logger.error(f"Failed to clear database: {str(e)}")
            return False

    def backup_database(self, backup_path: Optional[str] = None) -> bool:
        """
        Create a backup of the database.
        
        Args:
            backup_path: Optional path for backup file. If not provided,
                       uses default naming with timestamp.
                       
        Returns:
            bool: True if backup successful, False otherwise
        """
        if not backup_path:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            backup_path = str(self.db_path.parent / f"nba_data_backup_{timestamp}.db")
        
        try:
            import shutil
            shutil.copy2(self.db_path, backup_path)
            logger.info(f"Database backed up to {backup_path}")
            return True
        except Exception as e:
            logger.error(f"Backup failed: {str(e)}")
            return False 