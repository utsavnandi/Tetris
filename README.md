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

### Documentation

- **Public API reference (screens + tetris engine)**: `docs/API.md`
- **Screen/component behavior & transitions**: `docs/SCREENS.md`
- **Tetris engine notes & integration patterns**: `docs/TETRIS_ENGINE.md`

## How to build this game

This game was build using the [raylib game template](https://github.com/raysan5/raylib-game-template). Check the instructions [there](https://github.com/raysan5/raylib-game-template?tab=readme-ov-file#getting-started-with-this-template) to setup your development environment.

```bash
cd build
rm -rf *
cmake -G Ninja ..
ninja
```