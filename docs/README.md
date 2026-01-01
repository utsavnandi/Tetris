# Tetris Game Documentation

Complete API documentation, tutorials, and guides for the Tetris game project.

## 📚 Documentation Index

### Core Documentation

| Document | Description | Audience |
|----------|-------------|----------|
| [API Overview](./API_OVERVIEW.md) | High-level architecture and module overview | All developers |
| [Tetris Engine API](./TETRIS_API.md) | Complete game logic API reference | Game developers |
| [Screen System API](./SCREEN_API.md) | Screen management and UI system | UI/Frontend developers |
| [Examples & Tutorials](./EXAMPLES.md) | Code examples and step-by-step guides | Beginners |
| [Build Guide](./BUILD_GUIDE.md) | Build instructions for all platforms | DevOps/New developers |
| [Quick Reference](./QUICK_REFERENCE.md) | Concise API cheat sheet | All developers |

## 🚀 Quick Start

### For Users
Want to play the game? See [Build Guide](./BUILD_GUIDE.md#quick-start-linuxmacos) for installation instructions.

### For Developers
1. Read [API Overview](./API_OVERVIEW.md) for architecture understanding
2. Check [Build Guide](./BUILD_GUIDE.md#building-the-game) to set up your environment
3. Explore [Examples & Tutorials](./EXAMPLES.md#basic-tutorials) for hands-on learning
4. Reference [Tetris Engine API](./TETRIS_API.md) and [Screen System API](./SCREEN_API.md) as needed

### For Contributors
1. Set up development environment: [Development Setup](./BUILD_GUIDE.md#development-setup)
2. Review architecture: [API Overview](./API_OVERVIEW.md#architecture)
3. Follow code patterns: [Examples - Common Patterns](./EXAMPLES.md#common-patterns)

## 📖 Documentation Structure

```
docs/
├── README.md              # This file - documentation index
├── API_OVERVIEW.md        # Architecture and module overview
├── TETRIS_API.md          # Tetris game engine reference
├── SCREEN_API.md          # Screen system reference
├── EXAMPLES.md            # Code examples and tutorials
├── BUILD_GUIDE.md         # Build and setup instructions
└── QUICK_REFERENCE.md     # API quick reference card
```

## 🎯 Find What You Need

### I want to...

**...understand the project structure**
→ [API Overview - Architecture](./API_OVERVIEW.md#architecture)

**...learn the game logic**
→ [Tetris Engine API](./TETRIS_API.md)

**...add a new screen**
→ [Screen System API - Adding a New Screen](./SCREEN_API.md#adding-a-new-screen)

**...see code examples**
→ [Examples & Tutorials](./EXAMPLES.md)

**...build the game**
→ [Build Guide - Building the Game](./BUILD_GUIDE.md#building-the-game)

**...implement a feature**
→ [Examples - Advanced Examples](./EXAMPLES.md#advanced-examples)

**...fix a bug**
→ [Build Guide - Troubleshooting](./BUILD_GUIDE.md#troubleshooting-build-issues)

**...deploy the game**
→ [Build Guide - Deployment](./BUILD_GUIDE.md#deployment)

**...quickly look up an API**
→ [Quick Reference](./QUICK_REFERENCE.md)

## 🔍 API Reference Quick Links

### Tetris Engine

#### Core Functions
- [`gameState_new()`](./TETRIS_API.md#gamestate_new) - Create new game state
- [`startNewGame()`](./TETRIS_API.md#startnewgame) - Initialize game
- [`createTetromino()`](./TETRIS_API.md#createtetromino) - Spawn piece
- [`rotatePiece()`](./TETRIS_API.md#rotatepiece) - Rotate piece
- [`simulateMove()`](./TETRIS_API.md#simulatemove) - Validate move
- [`lockPieceInGrid()`](./TETRIS_API.md#lockpieceingrid) - Lock piece
- [`clearLines()`](./TETRIS_API.md#clearlines) - Clear completed lines

#### Data Types
- [`GameState`](./TETRIS_API.md#gamestate-struct) - Main game state
- [`Tetromino`](./TETRIS_API.md#tetromino-struct) - Piece instance
- [`MOVE`](./TETRIS_API.md#move-enum) - Movement types

### Screen System

#### Screen Lifecycle
- [`InitXXXScreen()`](./SCREEN_API.md#init-function) - Initialize screen
- [`UpdateXXXScreen()`](./SCREEN_API.md#update-function) - Update logic
- [`DrawXXXScreen()`](./SCREEN_API.md#draw-function) - Render screen
- [`UnloadXXXScreen()`](./SCREEN_API.md#unload-function) - Cleanup
- [`FinishXXXScreen()`](./SCREEN_API.md#finish-function) - Check transition

#### Screens
- [Logo Screen](./SCREEN_API.md#logo-screen) - Splash screen
- [Title Screen](./SCREEN_API.md#title-screen) - Main menu
- [Gameplay Screen](./SCREEN_API.md#gameplay-screen) - Main game
- [Ending Screen](./SCREEN_API.md#ending-screen) - Game over

## 📝 Code Examples

### Hello World - Minimal Tetris
```c
#include "tetris.h"

int main(void) {
    GameState *game = gameState_new();
    startNewGame(game);
    
    // Game ready to use!
    
    free(game);
    return 0;
}
```

**See more:** [Examples - Getting Started](./EXAMPLES.md#getting-started)

### Move a Piece
```c
// Try to move left
if (simulateMove(game, game->currentPiece, LEFT) == 0) {
    game->currentPiece.x -= 1;  // Move is valid
}
```

**See more:** [Examples - Tutorial 1](./EXAMPLES.md#tutorial-1-creating-and-moving-pieces)

### Add a New Screen
```c
void InitMyScreen(void) { /* initialize */ }
void UpdateMyScreen(void) { /* update logic */ }
void DrawMyScreen(void) { /* render */ }
void UnloadMyScreen(void) { /* cleanup */ }
int FinishMyScreen(void) { return finishFlag; }
```

**See more:** [Screen API - Implementation Guide](./SCREEN_API.md#implementation-guide)

## 🛠️ Common Tasks

### Building the Project

```bash
cd build
cmake -G Ninja ..
ninja
./raylib_game
```

**Full guide:** [Build Guide](./BUILD_GUIDE.md)

### Running Tests

```bash
cd build
ctest
```

### Generating Web Version

```bash
emcmake cmake ..
ninja
python3 -m http.server 8000
```

**Full guide:** [Build Guide - Web](./BUILD_GUIDE.md#web-emscripten)

## 🐛 Troubleshooting

Common issues and solutions:

- **Build fails:** [Troubleshooting Build Issues](./BUILD_GUIDE.md#troubleshooting-build-issues)
- **Piece collision bugs:** [Examples - Troubleshooting](./EXAMPLES.md#troubleshooting)
- **Screen transitions not working:** [Screen API - Transition System](./SCREEN_API.md#transition-system)

## 📊 Feature Matrix

| Feature | Implemented | Documented | Examples |
|---------|-------------|------------|----------|
| Basic Tetris gameplay | ✅ | ✅ | ✅ |
| Piece rotation | ✅ | ✅ | ✅ |
| Line clearing | ✅ | ✅ | ✅ |
| Collision detection | ✅ | ✅ | ✅ |
| Screen system | ✅ | ✅ | ✅ |
| Ghost piece | ❌ | ✅ | ✅ |
| Hold mechanic | ❌ | ✅ | ✅ |
| Piece preview | ❌ | ✅ | ✅ |
| Scoring system | ❌ | ✅ | ✅ |
| Wall kicks | ❌ | ✅ | ✅ |

Legend: ✅ Yes | ❌ No | 🔶 Partial

## 🤝 Contributing

Want to improve the documentation?

1. **Found a typo or error?** Submit an issue or PR
2. **Missing documentation?** Request it in an issue
3. **Want to add examples?** PRs welcome!

### Documentation Standards

- Use clear, concise language
- Include code examples for all APIs
- Provide both simple and advanced examples
- Test all code samples before committing
- Update this index when adding new docs

## 📜 License

This documentation is part of the Tetris game project and is licensed under the same terms (zlib/libpng license).

Copyright (c) 2025 Utsav Nandi

## 🔗 External Resources

### raylib Documentation
- [raylib Official Docs](https://www.raylib.com/)
- [raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raylib Examples](https://www.raylib.com/examples.html)

### C Programming
- [C Programming Language (K&R)](https://en.wikipedia.org/wiki/The_C_Programming_Language)
- [C Reference](https://en.cppreference.com/w/c)

### Build Tools
- [CMake Documentation](https://cmake.org/documentation/)
- [Ninja Build](https://ninja-build.org/)

### Tetris Guidelines
- [Tetris Guideline (Wikipedia)](https://en.wikipedia.org/wiki/Tetris#Guideline)
- [Super Rotation System](https://tetris.wiki/Super_Rotation_System)

## 📧 Contact

For questions about the documentation:
- Open an issue on GitHub
- Check existing documentation first
- Refer to [Examples & Tutorials](./EXAMPLES.md) for common questions

---

**Documentation Version:** 1.0  
**Last Updated:** 2025-01-01  
**Status:** Complete

*Happy coding! 🎮*
