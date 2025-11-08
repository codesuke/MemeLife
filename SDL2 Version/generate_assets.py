#!/usr/bin/env python3
"""
Generate placeholder assets for MemeLife game
"""
from PIL import Image, ImageDraw
import os

# Ensure assets directory exists
assets_dir = "assets"
sprites_dir = os.path.join(assets_dir, "sprites")
tilesets_dir = os.path.join(assets_dir, "tilesets")

# Create player sprite (32x32)
print("Creating player sprite...")
player_img = Image.new('RGBA', (32, 32), (255, 255, 255, 0))
draw = ImageDraw.Draw(player_img)

# Draw a simple character (blue circle with white outline)
draw.ellipse([8, 4, 24, 20], fill=(0, 100, 255), outline=(255, 255, 255), width=2)
# Draw body (blue rectangle)
draw.rectangle([10, 18, 22, 28], fill=(0, 100, 255), outline=(255, 255, 255), width=1)
# Draw legs
draw.rectangle([12, 28, 15, 32], fill=(0, 50, 200))
draw.rectangle([17, 28, 20, 32], fill=(0, 50, 200))

player_img.save(os.path.join(sprites_dir, "player.png"))
print(f"✓ Created {os.path.join(sprites_dir, 'player.png')}")

# Create tileset (64x32: floor tile and wall tile)
print("Creating tileset...")
tileset_img = Image.new('RGBA', (64, 32), (255, 255, 255, 0))
draw = ImageDraw.Draw(tileset_img)

# Floor tile (left side: 0-32)
# Light green with pattern
draw.rectangle([0, 0, 32, 32], fill=(144, 238, 144), outline=(100, 200, 100), width=2)
# Add a simple grid pattern
for i in range(0, 32, 8):
    draw.line([(i, 0), (i, 32)], fill=(100, 180, 100), width=1)
    draw.line([(0, i), (32, i)], fill=(100, 180, 100), width=1)

# Wall tile (right side: 32-64)
# Dark gray brick pattern
draw.rectangle([32, 0, 64, 32], fill=(80, 80, 80), outline=(50, 50, 50), width=2)
# Add brick pattern
draw.rectangle([36, 4, 60, 14], fill=(100, 100, 100))
draw.rectangle([36, 16, 60, 26], fill=(100, 100, 100))
# Brick lines
draw.line([(36, 15), (60, 15)], fill=(60, 60, 60), width=1)
draw.line([(48, 4), (48, 14)], fill=(60, 60, 60), width=1)
draw.line([(48, 16), (48, 26)], fill=(60, 60, 60), width=1)

tileset_img.save(os.path.join(tilesets_dir, "world_tileset.png"))
print(f"✓ Created {os.path.join(tilesets_dir, 'world_tileset.png')}")

print("\nAssets created successfully!")
print("Note: Background music file (theme.mp3 or theme.ogg) should be added manually to assets/music/")
