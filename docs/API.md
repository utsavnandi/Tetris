# API Reference

This project is a raylib-based game with two main public modules:

- `src/screens.h`: screen/component lifecycle API + shared globals used across screens
- `src/tetris.h`: minimal Tetris game-state + movement/rotation + line-clear logic

> In this codebase, “public API” means symbols declared in the public headers (`.h` files) and intended to be used by multiple `.c` modules.

---

## `screens.h` (Screen system / components)

### Types

#### `GameScreen`

```c
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;
```

- **Purpose**: Identifies which “screen component” is currently active.
- **Used by**: `src/raylib_game.c` (screen manager + transitions) and each `screen_*.c`.

### Shared globals

Declared in `src/screens.h`, defined in `src/raylib_game.c`.

- **`GameScreen currentScreen`**
  - **Meaning**: The currently active screen.
  - **Owner**: `raylib_game.c` screen manager; screens typically read it, and transitions update it.

- **`Font font`**
  - **Meaning**: Global font resource intended to be available for all screens.
  - **Loaded**: `raylib_game.c` via `LoadFont("resources/mecha.png")`
  - **Unloaded**: `raylib_game.c` via `UnloadFont(font)`

- **`Music music`**
  - **Meaning**: Global music stream (declared as shared; currently not loaded/used in the template).
  - **Note**: The template has calls commented out (`LoadMusicStream`, `PlayMusicStream`, `UnloadMusicStream`), but the variable exists so other screens/components can opt into using it.

- **`Sound fxCoin`**
  - **Meaning**: Global sound effect used across screens (e.g., button confirm, line clear).
  - **Loaded**: `raylib_game.c` via `LoadSound("resources/coin.wav")`
  - **Unloaded**: `raylib_game.c` via `UnloadSound(fxCoin)`

### Screen lifecycle functions (components)

Each screen implements a standard set of functions:

- `Init…Screen()` — allocate/init per-screen state
- `Update…Screen()` — update logic for one frame
- `Draw…Screen()` — render for one frame
- `Unload…Screen()` — free/unload per-screen state
- `Finish…Screen()` — return non-zero when the screen requests a transition

#### Logo screen

```c
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);
```

#### Title screen

```c
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);
```

#### Options screen

```c
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);
```

#### Gameplay screen

```c
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
```

#### Ending screen

```c
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);
```

### Usage: calling convention (how the screen manager uses screens)

The screen manager (in `src/raylib_game.c`) follows this pattern:

1. Load global resources (`font`, `fxCoin`, optionally `music`).
2. Initialize the first screen via `Init…Screen()`.
3. Each frame:
   - Call `Update…Screen()`
   - If `Finish…Screen()` indicates completion, transition to the next screen
   - Call `Draw…Screen()`
4. On exit or screen change, call the active screen’s `Unload…Screen()`.

### Example: adding a new screen component

1. Add a new `screen_<name>.c` implementing the 5 lifecycle functions.
2. Add a new enum value to `GameScreen` in `src/screens.h`.
3. Update the switch statements in `src/raylib_game.c`:
   - Initial init
   - Per-frame update/draw dispatch
   - Unload dispatch
   - Transition dispatch

---

## `tetris.h` (Tetris game logic module)

### Compile-time constants

```c
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
```

- **Meaning**: Dimensions of the playfield grid.
- **Used by**: `GameState.grid` and all collision/clear logic.

### Types

#### `MOVE`

```c
typedef enum MOVE {
    ROTATE,
    DOWN,
    LEFT,
    RIGHT
} MOVE;
```

- **Meaning**: Logical movement input for the active piece.

#### `GameStatus`

```c
typedef enum GameStatus {
    TO_START,
    RUNNING,
    GAME_OVER
} GameStatus;
```

- **Meaning**: High-level state for the game session.

#### `TetrominoDef`

```c
typedef struct {
    int shape[4][4];
} TetrominoDef;
```

- **Meaning**: An immutable 4×4 shape definition (the “template”).

#### `Tetromino`

```c
typedef struct {
    int shape[4][4];
    int x, y;
} Tetromino;
```

- **Meaning**: A concrete piece instance with position (`x`,`y`) in grid coordinates.

#### `GameState`

```c
typedef struct {
    int grid[GRID_HEIGHT][GRID_WIDTH];
    Tetromino currentPiece;
    GameStatus isGameRunning;
} GameState;
```

- **`grid`**: locked blocks (0 = empty, non-zero = filled)
- **`currentPiece`**: the active falling tetromino
- **`isGameRunning`**: coarse session state (`TO_START`, `RUNNING`, `GAME_OVER`)

### Global tetromino definitions

```c
extern const TetrominoDef TETROMINO_SQR;
extern const TetrominoDef TETROMINO_LINE;
extern const TetrominoDef TETROMINO_Z;
extern const TetrominoDef TETROMINO_RZ;
extern const TetrominoDef TETROMINO_T;
extern const TetrominoDef TETROMINO_L;
extern const TetrominoDef TETROMINO_RL;
extern const TetrominoDef TETROMINOS[7];
```

- **Defined in**: `src/tetris.c`
- **Meaning**: 7 canonical tetromino shapes plus an array for random selection.

### Functions

#### `GameState *gameState_new(void);`

- **What it does**: Allocates and zero-initializes a `GameState` with `isGameRunning = TO_START`.
- **Returns**: pointer you own (free with `free()`), or `NULL` on allocation failure.

#### `void startNewGame(GameState *state);`

- **What it does**:
  - Clears the grid
  - Sets `state->isGameRunning = RUNNING`
  - Seeds RNG with `srand(time(NULL))`
  - Spawns a first piece at `x = GRID_WIDTH / 2 - 2`, `y = 0` (with a 50% chance of being rotated once)
- **Preconditions**: `state != NULL`
- **Notes**: Because it seeds with `time(NULL)`, multiple calls within the same second can repeat sequences.

#### `Tetromino createTetromino(const TetrominoDef *def, int startX, int startY);`

- **What it does**: Copies `def->shape` into a `Tetromino` instance and sets `x/y`.
- **Returns**: a value-type `Tetromino` (no heap allocation).

#### `Tetromino rotatePiece(Tetromino piece);`

- **What it does**: Returns a rotated copy of `piece`.
- **Rotation direction**: anti-clockwise (as implemented in `src/tetris.c`).
- **Notes**: This does not perform collision checks; pair it with `checkPieceCollisionWithGrid()` or `simulateMove()`.

#### `int simulateMove(GameState *state, Tetromino piece, MOVE currentMove);`

- **What it does**: Applies `currentMove` to a *copy* of `piece` and checks the result against `state->grid` and bounds.
- **Return values**:
  - **For `LEFT`, `RIGHT`, `ROTATE`**: forwards `checkPieceCollisionWithGrid()`:
    - `0` = valid
    - `-1` = out of bounds
    - `-2` = collides with locked blocks
  - **For `DOWN`**: if the moved-down piece collides/out-of-bounds, it returns **`1`** (used as “can’t move down further”).

#### `int checkPieceCollisionWithGrid(GameState *state, Tetromino piece);`

- **What it does**: Checks whether `piece` would be out of bounds or overlap a non-zero grid cell.
- **Returns**:
  - `0` = valid placement
  - `-1` = out of bounds
  - `-2` = collides with locked blocks

#### `void lockPieceInGrid(GameState *state, Tetromino piece);`

- **What it does**: Writes the piece’s occupied cells into `state->grid` (marks filled cells as `1`).
- **Typical use**: When the active piece can’t move down anymore.

#### `int clearLines(GameState *state);`

- **What it does**: Clears any completely-filled rows and compacts rows above downward.
- **Returns**: number of lines cleared.

### Example: minimal “tick” loop (engine-only)

This example shows how to use the `tetris.h` API without raylib rendering:

```c
#include "tetris.h"
#include <stdio.h>

int main(void)
{
    GameState *g = gameState_new();
    if (!g) return 1;

    startNewGame(g);

    // Attempt to move down until we must lock.
    while (g->isGameRunning == RUNNING)
    {
        if (simulateMove(g, g->currentPiece, DOWN) == 0)
        {
            g->currentPiece.y += 1;
        }
        else
        {
            lockPieceInGrid(g, g->currentPiece);
            (void)clearLines(g);
            // Spawn next piece (simple random pick).
            int idx = rand() % 7;
            g->currentPiece = createTetromino(&TETROMINOS[idx], GRID_WIDTH/2 - 2, 0);
            if (simulateMove(g, g->currentPiece, DOWN) != 0) g->isGameRunning = GAME_OVER;
        }
    }

    puts("Game over!");
    free(g);
    return 0;
}
```

### Example: how gameplay currently uses the API

See `src/screen_gameplay.c` for a real integration:

- Movement:
  - Check `simulateMove(...) == 0`, then mutate `game->currentPiece.x/y`
- Rotation:
  - Build a rotated candidate via `rotatePiece(...)`, then validate before assigning
- Locking + clear:
  - On down collision, call `lockPieceInGrid(...)` then `clearLines(...)`

