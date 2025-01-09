from PIL import Image, ImageDraw, ImageFont
import os

def create_default_image(text, output_path, size=(260, 190)):
    # Create a new image with white background
    image = Image.new('RGB', size, 'white')
    draw = ImageDraw.Draw(image)
    
    # Draw text in the center
    try:
        font = ImageFont.truetype("Arial.ttf", 20)
    except:
        font = ImageFont.load_default()
    
    # Get text size
    text_bbox = draw.textbbox((0, 0), text, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    text_height = text_bbox[3] - text_bbox[1]
    
    # Calculate center position
    x = (size[0] - text_width) // 2
    y = (size[1] - text_height) // 2
    
    # Draw text
    draw.text((x, y), text, fill='black', font=font)
    
    # Save image
    image.save(output_path)

def main():
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    resources_dir = os.path.join(base_dir, 'resources')
    
    # Create default player image
    player_path = os.path.join(resources_dir, 'default_player.png')
    create_default_image("No Player Image", player_path)
    
    # Create default team logo
    team_path = os.path.join(resources_dir, 'default_team.png')
    create_default_image("No Team Logo", team_path)

if __name__ == "__main__":
    main() 