"""
SQLAlchemy database schema for NBA player and team data.
Defines the data models and relationships for storing NBA statistics,
player information, and team details.
"""

from sqlalchemy import create_engine, Column, Integer, String, Float, DateTime, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from datetime import datetime
from typing import Optional

Base = declarative_base()

class Team(Base):
    """
    NBA team information.
    
    Represents an NBA team with its basic information and relationships
    to players on the roster.
    
    Attributes:
        id (str): NBA team ID (e.g., "1610612747" for Lakers)
        name (str): Full team name (e.g., "Los Angeles Lakers")
        abbreviation (str): Team abbreviation (e.g., "LAL")
        conference (str): Conference name ("Eastern" or "Western")
        division (str): Division name
        logo_path (str): Path to team logo image file
        players (list): List of players on the team
    """
    
    __tablename__ = 'teams'
    
    id = Column(String, primary_key=True)
    name = Column(String)
    abbreviation = Column(String)
    conference = Column(String)
    division = Column(String)
    logo_path = Column(String)
    
    # Relationships
    players = relationship("Player", back_populates="team")
    
    def __repr__(self) -> str:
        return f"<Team {self.abbreviation}: {self.name}>"

class Player(Base):
    """
    NBA player information and relationships.
    
    Stores player personal information and maintains relationships
    to their team and statistics.
    
    Attributes:
        id (int): NBA player ID
        first_name (str): Player's first name
        last_name (str): Player's last name
        team_id (str): Foreign key to teams table
        headshot_path (str): Path to player's headshot image
        jersey_number (str): Player's jersey number
        position (str): Player's position
        height (str): Player's height
        weight (str): Player's weight
        team (Team): Relationship to team
        stats (PlayerStats): Relationship to player's statistics
    """
    
    __tablename__ = 'players'
    
    id = Column(Integer, primary_key=True)
    first_name = Column(String)
    last_name = Column(String)
    team_id = Column(String, ForeignKey('teams.id'))
    headshot_path = Column(String)
    jersey_number = Column(String)
    position = Column(String)
    height = Column(String)
    weight = Column(String)
    
    # Relationships
    team = relationship("Team", back_populates="players")
    stats = relationship("PlayerStats", back_populates="player", uselist=False)
    
    def __repr__(self) -> str:
        return f"<Player {self.id}: {self.first_name} {self.last_name}>"
    
    @property
    def full_name(self) -> str:
        """Get player's full name."""
        return f"{self.first_name} {self.last_name}"

class PlayerStats(Base):
    """
    NBA player statistics.
    
    Stores per-game statistics for a player in the current season.
    All percentage values are stored as decimals (e.g., 0.495 for 49.5%).
    
    Attributes:
        id (int): Primary key
        player_id (int): Foreign key to players table
        season (str): NBA season (e.g., "2024-25")
        games_played (int): Number of games played
        games_started (int): Number of games started
        minutes_per_game (float): Average minutes per game
        field_goal_percentage (float): Field goal percentage (0.0-1.0)
        three_point_percentage (float): Three-point percentage (0.0-1.0)
        free_throw_percentage (float): Free throw percentage (0.0-1.0)
        rebounds_per_game (float): Average rebounds per game
        assists_per_game (float): Average assists per game
        steals_per_game (float): Average steals per game
        blocks_per_game (float): Average blocks per game
        points_per_game (float): Average points per game
        last_updated (datetime): Timestamp of last update
    """
    
    __tablename__ = 'player_stats'
    
    id = Column(Integer, primary_key=True)
    player_id = Column(Integer, ForeignKey('players.id'))
    season = Column(String, nullable=False)
    games_played = Column(Integer)
    games_started = Column(Integer)
    minutes_per_game = Column(Float)
    field_goal_percentage = Column(Float)
    three_point_percentage = Column(Float)
    free_throw_percentage = Column(Float)
    rebounds_per_game = Column(Float)
    assists_per_game = Column(Float)
    steals_per_game = Column(Float)
    blocks_per_game = Column(Float)
    points_per_game = Column(Float)
    last_updated = Column(DateTime, default=datetime.now)
    
    # Relationships
    player = relationship("Player", back_populates="stats")
    
    def __repr__(self) -> str:
        return f"<PlayerStats {self.player_id}: {self.points_per_game}ppg>" 