# SDL2 Sprite Animation

## Overview
This program demonstrates sprite sheet animation using SDL2, cycling through frames of an alien sprite.

## Key Components

**Sprite Sheet Setup**
- Loads a sprite sheet image (`animation.png`)
- Divides the sheet into individual frames based on `ALIEN_FRAMES` (5 frames)
- Stores frame clipping rectangles in `gSpriteClips[]`

**Animation Loop**
```cpp
int frame = 0;
SDL_Rect* current_clip = &gSpriteClips[frame/FRAME_RATE_FACTOR];
gSheetTexture.render(0, 0, current_clip);
++frame;
```

**Frame Rate Control**
- Uses `FRAME_RATE_FACTOR` to slow down animation
- Dividing frame counter by this factor creates smooth playback

**Controls**
- Close window to exit
- Q/A, W/S, E/D: Adjust RGB color modulation
- O/P: Adjust alpha transparency




https://github.com/user-attachments/assets/684c37d7-d195-4b62-8c53-67a21467c13f

Yeah!, i get it, im not an animator. but you are a cool animator :P 
