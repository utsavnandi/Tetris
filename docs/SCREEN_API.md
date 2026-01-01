# Screen System API Documentation

## Table of Contents

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Common Interface](#common-interface)
4. [Global Variables](#global-variables)
5. [Screen Definitions](#screen-definitions)
6. [Transition System](#transition-system)
7. [Implementation Guide](#implementation-guide)
8. [Usage Examples](#usage-examples)

## Overview

The Screen System provides a modular architecture for managing different game states (Logo, Title, Options, Gameplay, Ending). Each screen implements a consistent interface for initialization, updating, rendering, and cleanup.

**Key Features:**
- Consistent lifecycle management across all screens
- Smooth fade transitions between screens
- Shared global resources (fonts, sounds)
- Clear separation between screen logic and main loop
- Easy to add new screens

## Architecture

### Screen Lifecycle

Each screen follows a four-phase lifecycle:

```
┌──────────┐
│   INIT   │  Initialize screen resources
└────┬─────┘
     │
     ▼
┌──────────┐
│  UPDATE  │  Process input, update logic (per frame)
└────┬─────┘
     │
     ▼
┌──────────┐
│   DRAW   │  Render screen (per frame)
└────┬─────┘
     │
     ▼
┌──────────┐
│  FINISH  │  Check if screen should transition
└────┬─────┘
     │
     ▼
┌──────────┐
│  UNLOAD  │  Clean up screen resources
└──────────┘
```

### File Structure

```
screens.h              # Public interface and declarations
raylib_game.c         # Main loop and transition management

# Screen Implementations:
screen_logo.c         # Logo/splash screen
screen_title.c        # Main menu
screen_options.c      # Settings (placeholder)
screen_gameplay.c     # Main game
screen_ending.c       # Game over/results
```

## Common Interface

Every screen must implement these five functions:

### Init Function

```c
void InitXXXScreen(void);
```

**Purpose:** Initialize all screen-specific resources and variables.

**Called:** When transitioning TO this screen.

**Responsibilities:**
- Allocate memory for screen state
- Load screen-specific assets
- Reset frame counters and state variables
- Set initial values

**Example:**
```c
void InitTitleScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
    // Initialize other screen variables
}
```

---

### Update Function

```c
void UpdateXXXScreen(void);
```

**Purpose:** Update screen logic every frame.

**Called:** Once per frame while screen is active.

**Responsibilities:**
- Handle user input
- Update animations and timers
- Update game state
- Trigger screen transitions via `finishScreen` variable

**Example:**
```c
void UpdateTitleScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        finishScreen = 2;  // Go to GAMEPLAY
        PlaySound(fxCoin);
    }
}
```

---

### Draw Function

```c
void DrawXXXScreen(void);
```

**Purpose:** Render screen visuals.

**Called:** Once per frame while screen is active.

**Responsibilities:**
- Clear/prepare canvas
- Draw all visual elements
- Render UI components
- Draw debug information (optional)

**Example:**
```c
void DrawTitleScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    DrawTextEx(font, "TITLE SCREEN", (Vector2){20, 10}, 
               font.baseSize*3.0f, 4, DARKGREEN);
}
```

---

### Unload Function

```c
void UnloadXXXScreen(void);
```

**Purpose:** Clean up screen-specific resources.

**Called:** When transitioning AWAY from this screen.

**Responsibilities:**
- Free allocated memory
- Unload screen-specific assets (NOT shared resources)
- Reset state for next time

**Example:**
```c
void UnloadGameplayScreen(void) {
    free(game);
    TraceLog(LOG_INFO, "Gameplay screen unloaded");
}
```

**Important:** Do NOT unload global/shared resources (font, music, fxCoin).

---

### Finish Function

```c
int FinishXXXScreen(void);
```

**Purpose:** Signal when to transition to another screen.

**Called:** Every frame by main loop.

**Returns:**
- `0`: Stay on current screen
- `1` (or other positive): Transition to specific screen

**Example:**
```c
int FinishTitleScreen(void) {
    return finishScreen;  // 0=stay, 1=options, 2=gameplay
}
```

## Global Variables

Defined in `screens.h`, implemented in `raylib_game.c`:

### currentScreen

```c
extern GameScreen currentScreen;
```

**Type:** `GameScreen` enum

**Description:** Tracks which screen is currently active.

**Values:**
```c
typedef enum GameScreen { 
    UNKNOWN = -1, 
    LOGO = 0, 
    TITLE, 
    OPTIONS, 
    GAMEPLAY, 
    ENDING 
} GameScreen;
```

**Usage:**
```c
if (currentScreen == GAMEPLAY) {
    // Gameplay-specific code
}
```

---

### font

```c
extern Font font;
```

**Type:** `Font` (raylib)

**Description:** Shared font resource loaded at startup.

**Initialization:**
```c
font = LoadFont("resources/mecha.png");
```

**Usage:**
```c
DrawTextEx(font, "Hello", (Vector2){10, 10}, font.baseSize*2, 2, WHITE);
```

---

### music

```c
extern Music music;
```

**Type:** `Music` (raylib)

**Description:** Background music stream (currently commented out in template).

**Usage:**
```c
// In main initialization:
music = LoadMusicStream("resources/music.ogg");
PlayMusicStream(music);

// In main loop:
UpdateMusicStream(music);
```

---

### fxCoin

```c
extern Sound fxCoin;
```

**Type:** `Sound` (raylib)

**Description:** Shared sound effect for interactions.

**Initialization:**
```c
fxCoin = LoadSound("resources/coin.wav");
```

**Usage:**
```c
if (IsKeyPressed(KEY_ENTER)) {
    PlaySound(fxCoin);
    finishScreen = 1;
}
```

## Screen Definitions

### Logo Screen

**File:** `src/screen_logo.c`

**Purpose:** Display animated raylib logo on startup.

**Features:**
- Animated logo construction
- Text fade-in effect
- Automatic transition to Title screen

**Functions:**
```c
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);
```

**Transition:** Automatically goes to `TITLE` after animation completes (~5 seconds).

**Animation States:**
1. **State 0**: Blinking corner
2. **State 1**: Top and left bars grow
3. **State 2**: Bottom and right bars grow
4. **State 3**: "raylib" text appears, then fade out

**Usage:**
```c
InitLogoScreen();
while (!FinishLogoScreen()) {
    UpdateLogoScreen();
    DrawLogoScreen();
}
```

---

### Title Screen

**File:** `src/screen_title.c`

**Purpose:** Main menu / title screen.

**Features:**
- Simple title display
- Press ENTER to start game
- Gesture support for mobile

**Functions:**
```c
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);
```

**Controls:**
- `ENTER` or Tap: Go to GAMEPLAY screen

**Return Values:**
- `0`: Stay on title
- `1`: Go to OPTIONS
- `2`: Go to GAMEPLAY

**Customization Points:**
```c
void DrawTitleScreen(void) {
    // Change background color
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), YOUR_COLOR);
    
    // Add menu buttons
    // Add version info
    // Add credits
}
```

---

### Options Screen

**File:** `src/screen_options.c`

**Purpose:** Game settings and configuration (placeholder).

**Status:** Template implementation - needs customization.

**Functions:**
```c
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);
```

**Suggested Features:**
- Volume controls
- Key binding configuration
- Graphics settings
- Difficulty selection

**Implementation Example:**
```c
typedef struct {
    float musicVolume;
    float sfxVolume;
    bool fullscreen;
    int difficulty;
} Options;

static Options options;

void InitOptionsScreen(void) {
    // Load saved options
    options = loadOptions();
}

void UpdateOptionsScreen(void) {
    // Handle option changes
    if (IsKeyPressed(KEY_UP)) options.musicVolume += 0.1f;
    if (IsKeyPressed(KEY_DOWN)) options.musicVolume -= 0.1f;
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        saveOptions(options);
        finishScreen = 1;  // Back to TITLE
    }
}
```

---

### Gameplay Screen

**File:** `src/screen_gameplay.c`

**Purpose:** Main Tetris gameplay.

**Features:**
- Full Tetris game integration
- Keyboard and gesture controls
- Grid rendering
- Sound effects on line clear
- Game over detection

**Functions:**
```c
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
```

**Controls:**
- `LEFT ARROW` / Swipe Left: Move piece left
- `RIGHT ARROW` / Swipe Right: Move piece right
- `UP ARROW` / Swipe Up: Rotate piece
- `DOWN ARROW` / Swipe Down: Move piece down (soft drop)
- `ESCAPE`: Pause/Exit

**State Management:**
```c
static GameState *game;  // Tetris game state

void InitGameplayScreen(void) {
    game = gameState_new();
    startNewGame(game);
}

void UnloadGameplayScreen(void) {
    free(game);
}
```

**Return Values:**
- `0`: Continue playing
- `1`: Go to ENDING screen (game over)

**Rendering:**
```c
void DrawGameplayScreen(void) {
    // Background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), LIGHTGRAY);
    
    // Grid borders
    DrawRectangleLines(80, 20, 400, 680, BLACK);
    
    // Locked pieces
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (game->grid[y][x] != 0) {
                DrawRectangle(/* calculate position */, BLUE);
            }
        }
    }
    
    // Current piece
    // ... render active tetromino
}
```

---

### Ending Screen

**File:** `src/screen_ending.c`

**Purpose:** Game over / results screen.

**Features:**
- Display final results
- Press ENTER to return to title

**Functions:**
```c
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);
```

**Controls:**
- `ENTER` or Tap: Return to TITLE screen

**Customization:**
```c
void InitEndingScreen(void) {
    // Retrieve final score from gameplay
    finalScore = getFinalScore();
    linesCleared = getTotalLines();
}

void DrawEndingScreen(void) {
    DrawText("GAME OVER", 200, 100, 40, WHITE);
    DrawText(TextFormat("Score: %d", finalScore), 200, 150, 30, WHITE);
    DrawText(TextFormat("Lines: %d", linesCleared), 200, 200, 30, WHITE);
}
```

## Transition System

The main game loop (`raylib_game.c`) manages screen transitions.

### Immediate Transition

```c
static void ChangeToScreen(int screen);
```

**Description:** Immediately switch to a new screen without animation.

**Process:**
1. Call current screen's `UnloadXXXScreen()`
2. Call new screen's `InitXXXScreen()`
3. Update `currentScreen` variable

**Usage:** Internal only (not exposed to screen implementations).

---

### Animated Transition

```c
static void TransitionToScreen(int screen);
```

**Description:** Switch to a new screen with fade effect.

**Process:**
1. Fade out current screen (black overlay increases)
2. When fully faded, unload current and init next
3. Fade in new screen (black overlay decreases)

**Duration:** ~2 seconds

**Implementation:**
```c
// Fade out: transAlpha goes 0.0 → 1.0
// At 1.0: switch screens
// Fade in: transAlpha goes 1.0 → 0.0
```

**Usage in Screen:**
```c
void UpdateTitleScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        finishScreen = 2;  // Signal transition needed
    }
}

// Main loop handles the actual transition:
if (FinishTitleScreen() == 2) {
    TransitionToScreen(GAMEPLAY);
}
```

---

### Main Loop Logic

```c
static void UpdateDrawFrame(void) {
    // Update
    if (!onTransition) {
        switch(currentScreen) {
            case TITLE:
                UpdateTitleScreen();
                if (FinishTitleScreen() == 2) 
                    TransitionToScreen(GAMEPLAY);
                break;
            // ... other screens
        }
    } else {
        UpdateTransition();  // Handle fade animation
    }
    
    // Draw
    BeginDrawing();
        switch(currentScreen) {
            case TITLE: DrawTitleScreen(); break;
            // ... other screens
        }
        
        if (onTransition) DrawTransition();  // Fade overlay
    EndDrawing();
}
```

## Implementation Guide

### Adding a New Screen

**Step 1:** Add enum value to `screens.h`

```c
typedef enum GameScreen { 
    UNKNOWN = -1, 
    LOGO = 0, 
    TITLE, 
    OPTIONS, 
    GAMEPLAY,
    PAUSE,      // ← New screen
    ENDING 
} GameScreen;
```

**Step 2:** Declare functions in `screens.h`

```c
void InitPauseScreen(void);
void UpdatePauseScreen(void);
void DrawPauseScreen(void);
void UnloadPauseScreen(void);
int FinishPauseScreen(void);
```

**Step 3:** Create implementation file `screen_pause.c`

```c
#include "raylib.h"
#include "screens.h"

static int framesCounter = 0;
static int finishScreen = 0;

void InitPauseScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
}

void UpdatePauseScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        finishScreen = 1;  // Resume
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        finishScreen = 2;  // Quit to title
    }
}

void DrawPauseScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                  Fade(BLACK, 0.5f));
    DrawText("PAUSED", 300, 300, 40, WHITE);
    DrawText("ENTER: Resume | ESC: Quit", 200, 350, 20, WHITE);
}

void UnloadPauseScreen(void) {
    // Cleanup if needed
}

int FinishPauseScreen(void) {
    return finishScreen;
}
```

**Step 4:** Integrate into main loop (`raylib_game.c`)

```c
// In UpdateDrawFrame() UPDATE section:
case PAUSE:
    UpdatePauseScreen();
    if (FinishPauseScreen() == 1) TransitionToScreen(GAMEPLAY);
    else if (FinishPauseScreen() == 2) TransitionToScreen(TITLE);
    break;

// In UpdateDrawFrame() DRAW section:
case PAUSE: DrawPauseScreen(); break;

// In ChangeToScreen() and TransitionToScreen():
case PAUSE: InitPauseScreen(); break;    // Init
case PAUSE: UnloadPauseScreen(); break;  // Unload
```

**Step 5:** Add build system entry

In `CMakeLists.txt` or `Makefile`:
```cmake
add_executable(raylib_game
    src/raylib_game.c
    src/screen_logo.c
    src/screen_title.c
    src/screen_pause.c    # ← Add this
    # ...
)
```

### Screen Design Best Practices

1. **Keep State Local:** Use `static` variables in screen files
2. **Don't Block:** Update functions should return quickly (no loops)
3. **Cleanup Resources:** Always free memory in Unload
4. **Use Shared Assets:** Reuse global font and sounds when possible
5. **Smooth Transitions:** Use fade effects or gradual animations
6. **Handle All Inputs:** Provide escape mechanisms (ESC key)
7. **Mobile Support:** Include gesture support for touch screens

### Common Patterns

**Frame-based Animation:**
```c
static int framesCounter = 0;

void UpdateMyScreen(void) {
    framesCounter++;
    
    if (framesCounter % 60 == 0) {  // Every second at 60 FPS
        // Do something periodic
    }
}
```

**State Machine:**
```c
static int screenState = 0;

void UpdateMyScreen(void) {
    switch (screenState) {
        case 0: // Intro
            if (framesCounter > 120) screenState = 1;
            break;
        case 1: // Menu
            if (IsKeyPressed(KEY_ENTER)) screenState = 2;
            break;
        case 2: // Exit
            finishScreen = 1;
            break;
    }
}
```

**Alpha Fading:**
```c
static float alpha = 1.0f;

void UpdateMyScreen(void) {
    alpha -= 0.02f;
    if (alpha <= 0.0f) finishScreen = 1;
}

void DrawMyScreen(void) {
    DrawText("Fading...", 100, 100, 40, Fade(WHITE, alpha));
}
```

## Usage Examples

### Complete Screen Implementation

```c
/**********************************************************************************************
*   Custom Screen - High Score Display
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <stdio.h>

// Module Variables
static int framesCounter = 0;
static int finishScreen = 0;
static int highScores[10];
static float scrollY = 0.0f;

// Load high scores from file
static void LoadHighScores(void) {
    FILE *f = fopen("highscores.dat", "rb");
    if (f) {
        fread(highScores, sizeof(int), 10, f);
        fclose(f);
    } else {
        // Default scores
        for (int i = 0; i < 10; i++) {
            highScores[i] = (10 - i) * 1000;
        }
    }
}

void InitHighScoreScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
    scrollY = 0.0f;
    LoadHighScores();
    TraceLog(LOG_INFO, "High score screen initialized");
}

void UpdateHighScoreScreen(void) {
    framesCounter++;
    
    // Auto-scroll
    scrollY += 0.5f;
    if (scrollY > 50.0f) scrollY = 0.0f;
    
    // Exit on any key
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

void DrawHighScoreScreen(void) {
    ClearBackground(DARKBLUE);
    
    // Title
    DrawTextEx(font, "HIGH SCORES", (Vector2){150, 50 - scrollY}, 
               font.baseSize * 3, 4, GOLD);
    
    // Score list
    for (int i = 0; i < 10; i++) {
        int y = 150 + i * 50 - (int)scrollY;
        DrawText(TextFormat("%2d. %08d", i + 1, highScores[i]), 
                 200, y, 30, WHITE);
    }
    
    // Instructions
    DrawText("Press ENTER to continue", 220, 700, 20, LIGHTGRAY);
}

void UnloadHighScoreScreen(void) {
    TraceLog(LOG_INFO, "High score screen unloaded");
}

int FinishHighScoreScreen(void) {
    return finishScreen;
}
```

### Screen Communication

To pass data between screens, use static global variables or files:

```c
// In screen_gameplay.c
static int finalScore = 0;
static int totalLines = 0;

int GetGameplayScore(void) { return finalScore; }
int GetGameplayLines(void) { return totalLines; }

// In screen_ending.c
extern int GetGameplayScore(void);
extern int GetGameplayLines(void);

void InitEndingScreen(void) {
    int score = GetGameplayScore();
    int lines = GetGameplayLines();
    // Display results
}
```

Or use a shared structure:

```c
// In screens.h
typedef struct GameResults {
    int score;
    int lines;
    int level;
    float timeElapsed;
} GameResults;

extern GameResults lastGameResults;

// Gameplay sets it, Ending reads it
```

---

**Last Updated:** 2025-01-01  
**API Version:** 1.0  
**Author:** Utsav Nandi
