# Tetris Game - API Documentation Overview

## Table of Contents

1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [Core Modules](#core-modules)
4. [Quick Start](#quick-start)
5. [Further Reading](#further-reading)

## Introduction

This is a classic Tetris game implementation built with the raylib game framework. The codebase is organized into modular components following a screen-based architecture pattern, separating game logic from rendering and user interface concerns.

**Key Features:**
- Classic Tetris gameplay mechanics
- Multi-screen architecture (Logo, Title, Options, Gameplay, Ending)
- Smooth screen transitions with fade effects
- Keyboard and gesture input support
- Sound effects and music support
- Cross-platform compatibility (Desktop, Web via Emscripten)

## Architecture

The game follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────┐
│     Main Game Loop (raylib_game.c)  │
│  - Window management                │
│  - Screen transitions                │
│  - Global resource loading           │
└──────────┬──────────────────────────┘
           │
           ├──────────────────────────┐
           │                          │
┌──────────▼────────┐      ┌─────────▼──────────┐
│  Screen System    │      │  Tetris Engine     │
│  (screens.h)      │      │  (tetris.h)        │
│                   │      │                    │
│  - Logo           │      │  - Game State      │
│  - Title          │      │  - Tetromino Logic │
│  - Options        │      │  - Collision       │
│  - Gameplay       │      │  - Line Clearing   │
│  - Ending         │      │                    │
└───────────────────┘      └────────────────────┘
```

### Design Patterns

1. **Screen State Pattern**: Each screen implements a consistent interface with Init, Update, Draw, Unload, and Finish functions
2. **Global Shared Resources**: Common assets (fonts, sounds) are loaded once and shared across screens
3. **Separation of Concerns**: Game logic (tetris.c) is independent from rendering (screen_gameplay.c)

## Core Modules

### 1. Main Game Loop (`src/raylib_game.c`)

The entry point and orchestrator of the entire game. Handles:
- Window initialization
- Screen state management
- Global asset loading/unloading
- Frame-by-frame update and rendering
- Screen transitions with fade effects

**Key Functions:**
- `main()` - Program entry point
- `UpdateDrawFrame()` - Main game loop iteration
- `ChangeToScreen()` - Immediate screen switching
- `TransitionToScreen()` - Animated screen transitions

### 2. Screen System (`src/screens.h`)

Defines the interface for all game screens and shared global variables.

**Screens:**
- **Logo Screen** - Animated raylib logo on startup
- **Title Screen** - Main menu
- **Options Screen** - Game settings (placeholder)
- **Gameplay Screen** - Main Tetris gameplay
- **Ending Screen** - Game over screen

**Each Screen Provides:**
- `InitXXXScreen()` - Initialize screen resources
- `UpdateXXXScreen()` - Update screen logic per frame
- `DrawXXXScreen()` - Render screen visuals
- `UnloadXXXScreen()` - Clean up screen resources
- `FinishXXXScreen()` - Check if screen should transition

### 3. Tetris Game Engine (`src/tetris.h`, `src/tetris.c`)

Core game logic independent of rendering. Implements:
- Tetromino shapes and rotation
- Grid-based collision detection
- Piece locking and line clearing
- Game state management

**See:** [Tetris Engine API Documentation](./TETRIS_API.md)

## Quick Start

### Building the Game

```bash
cd build
rm -rf *
cmake -G Ninja ..
ninja
```

### Basic Integration Example

```c
#include "tetris.h"

// Create new game state
GameState *game = gameState_new();

// Start a new game
startNewGame(game);

// Game loop
while (game->isGameRunning == RUNNING) {
    // Check user input and simulate move
    if (simulateMove(game, game->currentPiece, DOWN) == 0) {
        game->currentPiece.y += 1;  // Move is valid
    } else {
        // Lock piece and generate new one
        lockPieceInGrid(game, game->currentPiece);
        clearLines(game);
        // ... spawn new piece
    }
}

// Cleanup
free(game);
```

## Further Reading

- [Tetris Engine API](./TETRIS_API.md) - Detailed game logic documentation
- [Screen System API](./SCREEN_API.md) - Screen implementation guide
- [Examples and Tutorials](./EXAMPLES.md) - Code examples and usage patterns
- [Build Guide](./BUILD_GUIDE.md) - Detailed build instructions

## Project Structure

```
/workspace/
├── src/
│   ├── raylib_game.c          # Main entry point
│   ├── screens.h              # Screen system interface
│   ├── tetris.h               # Tetris engine interface
│   ├── tetris.c               # Tetris engine implementation
│   ├── screen_logo.c          # Logo screen
│   ├── screen_title.c         # Title screen
│   ├── screen_options.c       # Options screen
│   ├── screen_gameplay.c      # Gameplay screen
│   ├── screen_ending.c        # Ending screen
│   └── resources/             # Game assets
├── docs/                      # Documentation (this directory)
└── CMakeLists.txt            # Build configuration
```

## License

This game is licensed under the zlib/libpng license. See [LICENSE](../LICENSE) for details.

Copyright (c) 2025 Utsav Nandi
