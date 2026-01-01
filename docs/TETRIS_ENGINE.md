# Tetris Engine (`tetris.h`)

The `src/tetris.h` module provides a small “engine” for:

- piece definitions (`TetrominoDef`)
- piece instances (`Tetromino`)
- a locked-block grid + active piece (`GameState`)
- movement/rotation simulation + collision checks
- line clearing

It is intentionally minimal: scoring, levels, gravity/timers, hold/next queue, wall kicks, etc. are not implemented.

---

## Coordinate system

- The playfield is a `GRID_HEIGHT × GRID_WIDTH` grid (`20 × 10`).
- A `Tetromino` has an `(x, y)` origin in **grid coordinates**.
- The tetromino occupies a `4×4` local mask; for each occupied local cell `(px, py)`, the occupied grid cell is:

\[
gridX = x + px,\quad gridY = y + py
\]

---

## Common integration patterns

### Validating moves before applying them

`simulateMove()` is designed to be used as a “probe”:

1. Ask if a move is valid.
2. If valid, mutate `game->currentPiece` yourself.

Example (left):

```c
if (simulateMove(game, game->currentPiece, LEFT) == 0) {
    game->currentPiece.x -= 1;
}
```

### Rotation

Rotation is two-step:

1. Create a rotated candidate using `rotatePiece()`.
2. Validate the rotated candidate before assigning it.

```c
Tetromino rotated = rotatePiece(game->currentPiece);
if (simulateMove(game, rotated, ROTATE) == 0) {
    game->currentPiece = rotated;
}
```

### Lock + clear + spawn

In this project’s gameplay, “can’t move down” means “lock it”:

```c
if (simulateMove(game, game->currentPiece, DOWN) != 0) {
    lockPieceInGrid(game, game->currentPiece);
    int cleared = clearLines(game);
    // spawn a new piece...
}
```

---

## Important behavioral notes (current implementation)

- **`simulateMove(..., DOWN)` returns `1` on any downward collision/out-of-bounds**, rather than returning `-1` or `-2`. This is treated as a “stop falling and lock” signal.
- **No automatic gravity**: the current gameplay screen only advances the piece downward on explicit down input (or swipe-down).
- **Grid cell values**: `lockPieceInGrid()` currently writes `1` for filled cells, so there’s no per-piece color/type in the grid yet.
- **RNG seeding**: `startNewGame()` calls `srand(time(NULL))`. Re-starting multiple times in the same second can repeat the sequence.

