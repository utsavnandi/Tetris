## Tetris

![Tetris](screenshots/screenshot000.png "Tetris")

### Description

Tetris is a classic tile-matching puzzle game where players must strategically place falling tetrominoes to create and clear complete lines.

### Features

 - Classic Tetris gameplay
 - Smooth controls with keyboard and gesture support
 - Made with raylib

### Controls

Keyboard:
 - Left Arrow: Move piece left
 - Right Arrow: Move piece right
 - Up Arrow: Rotate piece
 - Down Arrow: Move piece down faster
 - Escape: Pause/Exit game

### Screenshots

Work in progress...


### License

This game sources are licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

*Copyright (c) 2025 Utsav Nandi (utsavnandi)*

## Documentation

**📚 [Complete API Documentation](docs/README.md)**

Comprehensive documentation for developers:

- **[API Overview](docs/API_OVERVIEW.md)** - Architecture and module overview
- **[Tetris Engine API](docs/TETRIS_API.md)** - Complete game logic reference
- **[Screen System API](docs/SCREEN_API.md)** - Screen management system
- **[Examples & Tutorials](docs/EXAMPLES.md)** - Code examples and guides
- **[Build Guide](docs/BUILD_GUIDE.md)** - Build instructions for all platforms

## Quick Start

### Building the Game

```bash
# Create build directory
mkdir -p build && cd build

# Configure and build
cmake -G Ninja ..
ninja

# Run the game
./raylib_game
```

**For detailed build instructions, platform-specific guides, and troubleshooting:**  
See the [Build Guide](docs/BUILD_GUIDE.md)

### For Developers

This game is built using the [raylib game template](https://github.com/raysan5/raylib-game-template) with a modular architecture:

- **Tetris Engine** (`src/tetris.c/h`) - Pure game logic, renderer-independent
- **Screen System** (`src/screens.h`) - Modular UI state management
- **Game Loop** (`src/raylib_game.c`) - Main entry point and orchestration

**Getting Started as a Developer:**
1. Read [API Overview](docs/API_OVERVIEW.md) to understand the architecture
2. Check [Examples & Tutorials](docs/EXAMPLES.md) for hands-on guides
3. Refer to API documentation as needed:
   - [Tetris Engine API](docs/TETRIS_API.md) for game logic
   - [Screen System API](docs/SCREEN_API.md) for UI screens

### Adding Features

**Example: Add a ghost piece preview**
```c
Tetromino ghost = game->currentPiece;
while (simulateMove(game, ghost, DOWN) == 0) {
    ghost.y += 1;
}
// Render ghost piece with transparency
```

**More examples:** [docs/EXAMPLES.md#advanced-examples](docs/EXAMPLES.md#advanced-examples)