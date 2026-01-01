# Tetris Game Engine API Documentation

## Table of Contents

1. [Overview](#overview)
2. [Constants](#constants)
3. [Data Types](#data-types)
4. [Global Variables](#global-variables)
5. [Functions](#functions)
6. [Usage Examples](#usage-examples)
7. [Implementation Notes](#implementation-notes)

## Overview

The Tetris game engine (`tetris.h` / `tetris.c`) provides a complete implementation of Tetris game logic independent of any rendering system. It handles piece generation, movement, collision detection, grid management, and line clearing.

**Key Capabilities:**
- 7 classic tetromino shapes
- Grid-based game state (10×20 cells)
- Collision detection (boundaries and locked pieces)
- Piece rotation (anti-clockwise)
- Line clearing with gravity
- Game over detection

## Constants

### Grid Dimensions

```c
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
```

**Description:**
- Standard Tetris grid dimensions
- `GRID_WIDTH`: Number of cells horizontally (10)
- `GRID_HEIGHT`: Number of cells vertically (20)

**Usage:**
```c
// Iterate over entire grid
for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        // Access cell at position (x, y)
    }
}
```

## Data Types

### MOVE (Enum)

```c
typedef enum MOVE {
    ROTATE,    // Rotate piece anti-clockwise
    DOWN,      // Move piece down
    LEFT,      // Move piece left
    RIGHT      // Move piece right
} MOVE;
```

**Description:** Represents the different move actions a player can perform on the current piece.

**Usage:**
```c
// Test if moving left is valid
if (simulateMove(game, game->currentPiece, LEFT) == 0) {
    // Move is valid
}
```

---

### GameStatus (Enum)

```c
typedef enum GameStatus {
    TO_START,   // Game hasn't started yet
    RUNNING,    // Game is active
    GAME_OVER   // Game has ended
} GameStatus;
```

**Description:** Tracks the current state of the game.

**Usage:**
```c
if (game->isGameRunning == GAME_OVER) {
    // Display game over screen
}
```

---

### TetrominoDef (Struct)

```c
typedef struct {
    int shape[4][4];  // 4x4 grid representing the piece shape
} TetrominoDef;
```

**Description:** Defines the shape of a tetromino as a 4×4 matrix. Each cell is 0 (empty) or 1 (filled).

**Example:**
```c
// L-shaped piece
const TetrominoDef TETROMINO_L = {
    .shape = {
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};
```

---

### Tetromino (Struct)

```c
typedef struct {
    int shape[4][4];  // Current shape of the piece
    int x, y;         // Position on the grid (top-left corner)
} Tetromino;
```

**Description:** Represents an active tetromino piece with its current shape and position.

**Fields:**
- `shape[4][4]`: 4×4 matrix defining the piece's current rotation
- `x`: Horizontal position (0 = leftmost column)
- `y`: Vertical position (0 = top row)

**Usage:**
```c
Tetromino piece = createTetromino(&TETROMINO_T, 3, 0);
piece.x += 1;  // Move right by one cell
```

---

### GameState (Struct)

```c
typedef struct {
    int grid[GRID_HEIGHT][GRID_WIDTH];  // Grid of locked pieces
    Tetromino currentPiece;              // Active falling piece
    GameStatus isGameRunning;            // Current game status
} GameState;
```

**Description:** Contains the complete state of a Tetris game.

**Fields:**
- `grid[GRID_HEIGHT][GRID_WIDTH]`: 2D array where 0 = empty, 1 = locked piece
- `currentPiece`: The piece currently being controlled by the player
- `isGameRunning`: Current game status (TO_START, RUNNING, GAME_OVER)

**Memory Management:**
```c
// Create
GameState *game = gameState_new();

// Use
startNewGame(game);

// Destroy
free(game);
```

## Global Variables

### Tetromino Definitions

```c
extern const TetrominoDef TETROMINO_SQR;    // Square (O-piece)
extern const TetrominoDef TETROMINO_LINE;   // Line (I-piece)
extern const TetrominoDef TETROMINO_Z;      // Z-piece
extern const TetrominoDef TETROMINO_RZ;     // Reverse Z (S-piece)
extern const TetrominoDef TETROMINO_T;      // T-piece
extern const TetrominoDef TETROMINO_L;      // L-piece
extern const TetrominoDef TETROMINO_RL;     // Reverse L (J-piece)
extern const TetrominoDef TETROMINOS[7];    // Array of all pieces
```

**Description:** Pre-defined tetromino shapes for all 7 classic Tetris pieces.

**Shape Definitions:**

| Piece | Shape | Representation |
|-------|-------|----------------|
| `TETROMINO_SQR` | Square | `■■`<br>`■■` |
| `TETROMINO_LINE` | Line | `■■■■` |
| `TETROMINO_Z` | Z | `■■·`<br>`·■■` |
| `TETROMINO_RZ` | S | `·■■`<br>`■■·` |
| `TETROMINO_T` | T | `·■·`<br>`■■■` |
| `TETROMINO_L` | L | `■··`<br>`■■■` |
| `TETROMINO_RL` | J | `··■`<br>`■■■` |

**Usage:**
```c
// Randomly select a piece
int index = rand() % 7;
Tetromino piece = createTetromino(&TETROMINOS[index], GRID_WIDTH/2 - 2, 0);
```

## Functions

### gameState_new

```c
GameState *gameState_new(void);
```

**Description:** Allocates and initializes a new game state.

**Returns:**
- `GameState*`: Pointer to newly allocated game state
- `NULL`: If allocation fails

**Usage:**
```c
GameState *game = gameState_new();
if (game == NULL) {
    fprintf(stderr, "Failed to allocate game state\n");
    return -1;
}
```

**Memory:** Caller is responsible for freeing the returned pointer with `free()`.

**Initial State:**
- Grid is cleared (all zeros)
- Game status is `TO_START`

---

### startNewGame

```c
void startNewGame(GameState *state);
```

**Description:** Initializes a new game by clearing the grid and spawning the first piece.

**Parameters:**
- `state`: Pointer to the game state to initialize

**Effects:**
- Clears all locked pieces from the grid
- Sets game status to `RUNNING`
- Generates and positions the first random tetromino
- May randomly rotate the first piece

**Usage:**
```c
GameState *game = gameState_new();
startNewGame(game);  // Ready to play!
```

**Note:** Seeds the random number generator with `srand(time(NULL))`.

---

### createTetromino

```c
Tetromino createTetromino(const TetrominoDef *def, int startX, int startY);
```

**Description:** Creates a new tetromino instance from a definition at a specific position.

**Parameters:**
- `def`: Pointer to tetromino definition (shape)
- `startX`: Initial horizontal position on grid
- `startY`: Initial vertical position on grid

**Returns:**
- `Tetromino`: New tetromino instance

**Usage:**
```c
// Spawn T-piece at top center
Tetromino piece = createTetromino(&TETROMINO_T, GRID_WIDTH/2 - 2, 0);
```

**Common Starting Position:**
```c
// Standard spawn position (centered at top)
int spawnX = GRID_WIDTH / 2 - 2;  // Center (x=3 for 10-wide grid)
int spawnY = 0;                    // Top of grid
```

---

### rotatePiece

```c
Tetromino rotatePiece(Tetromino piece);
```

**Description:** Rotates a tetromino 90 degrees anti-clockwise.

**Parameters:**
- `piece`: Tetromino to rotate (passed by value)

**Returns:**
- `Tetromino`: New rotated tetromino

**Algorithm:**
- Performs matrix transposition and reversal
- Rotation formula: `rotated[3-x][y] = original[y][x]`

**Usage:**
```c
Tetromino rotated = rotatePiece(game->currentPiece);

// Validate before applying
if (checkPieceCollisionWithGrid(game, rotated) == 0) {
    game->currentPiece = rotated;  // Valid rotation
}
```

**Example Rotation:**
```
T-piece:        After rotation:
 ·■·             ·■·
 ■■■      →      ·■■
 ···             ·■·
```

---

### simulateMove

```c
int simulateMove(GameState *state, Tetromino piece, MOVE currentMove);
```

**Description:** Tests whether a move is valid without modifying game state.

**Parameters:**
- `state`: Current game state
- `piece`: Piece to test movement on
- `currentMove`: Type of move to simulate (DOWN, LEFT, RIGHT, ROTATE)

**Returns:**
- `0`: Move is valid
- `-1`: Invalid - would go out of bounds
- `-2`: Invalid - would collide with locked piece
- `1`: Special case - DOWN move hits bottom/piece (piece should lock)

**Usage:**
```c
// Test if piece can move left
int result = simulateMove(game, game->currentPiece, LEFT);
if (result == 0) {
    game->currentPiece.x -= 1;  // Apply the move
}

// Test if piece should lock
if (simulateMove(game, game->currentPiece, DOWN) == 1) {
    lockPieceInGrid(game, game->currentPiece);
}
```

**Important:** This function does NOT modify the game state. You must manually apply valid moves.

---

### checkPieceCollisionWithGrid

```c
int checkPieceCollisionWithGrid(GameState *state, Tetromino piece);
```

**Description:** Checks if a piece collides with grid boundaries or locked pieces.

**Parameters:**
- `state`: Current game state
- `piece`: Piece to check for collisions

**Returns:**
- `0`: No collision (valid position)
- `-1`: Out of bounds
- `-2`: Collides with locked piece

**Algorithm:**
1. Iterates through piece's 4×4 shape
2. For each filled cell, calculates grid position
3. Checks if position is within bounds and not occupied

**Usage:**
```c
Tetromino testPiece = game->currentPiece;
testPiece.y += 1;  // Try moving down

if (checkPieceCollisionWithGrid(game, testPiece) == 0) {
    // Safe to move down
    game->currentPiece = testPiece;
}
```

**Use Cases:**
- Validating player moves
- Detecting game over condition (new piece spawns in invalid position)
- Testing rotations (wall kicks not implemented)

---

### lockPieceInGrid

```c
void lockPieceInGrid(GameState *state, Tetromino piece);
```

**Description:** Permanently places a piece into the grid by marking its cells as filled.

**Parameters:**
- `state`: Game state to modify
- `piece`: Piece to lock in place

**Effects:**
- All filled cells in the piece shape are written to the grid
- Grid cells are set to `1` (occupied)
- Original piece is not modified

**Usage:**
```c
// When piece can't move down further
if (simulateMove(game, game->currentPiece, DOWN) != 0) {
    lockPieceInGrid(game, game->currentPiece);
    
    // Clear any complete lines
    int cleared = clearLines(game);
    
    // Spawn next piece
    // ...
}
```

**Important:** After locking, you should:
1. Call `clearLines()` to remove completed rows
2. Spawn a new piece
3. Check for game over

---

### clearLines

```c
int clearLines(GameState *state);
```

**Description:** Removes all complete horizontal lines and shifts remaining lines down.

**Parameters:**
- `state`: Game state to modify

**Returns:**
- `int`: Number of lines cleared (0-4)

**Algorithm:**
1. Scans from bottom to top
2. Identifies complete rows (all cells filled)
3. Shifts non-complete rows down by the number of cleared lines
4. Clears the top rows

**Usage:**
```c
lockPieceInGrid(game, game->currentPiece);

int linesCleared = clearLines(game);
if (linesCleared > 0) {
    // Award points, play sound, etc.
    int score = calculateScore(linesCleared);
}
```

**Line Clear Scoring (Suggested):**
- 1 line: 100 points
- 2 lines: 300 points
- 3 lines: 500 points
- 4 lines (Tetris): 800 points

**Performance:** O(GRID_WIDTH × GRID_HEIGHT) = O(200) operations

## Usage Examples

### Complete Game Loop

```c
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    // Initialize game
    GameState *game = gameState_new();
    if (!game) {
        fprintf(stderr, "Failed to create game state\n");
        return 1;
    }
    
    startNewGame(game);
    
    // Main game loop
    while (game->isGameRunning == RUNNING) {
        // Get player input (pseudo-code)
        MOVE move = getPlayerInput();
        
        // Try to apply the move
        if (simulateMove(game, game->currentPiece, move) == 0) {
            switch (move) {
                case LEFT:  game->currentPiece.x -= 1; break;
                case RIGHT: game->currentPiece.x += 1; break;
                case DOWN:  game->currentPiece.y += 1; break;
                case ROTATE:
                    game->currentPiece = rotatePiece(game->currentPiece);
                    break;
            }
        }
        
        // Auto-drop piece (gravity)
        if (simulateMove(game, game->currentPiece, DOWN) == 1) {
            // Piece has landed
            lockPieceInGrid(game, game->currentPiece);
            
            int linesCleared = clearLines(game);
            printf("Cleared %d lines\n", linesCleared);
            
            // Spawn new piece
            int randIdx = rand() % 7;
            game->currentPiece = createTetromino(
                &TETROMINOS[randIdx], 
                GRID_WIDTH / 2 - 2, 
                0
            );
            
            // Check game over
            if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
                game->isGameRunning = GAME_OVER;
                printf("Game Over!\n");
            }
        }
    }
    
    // Cleanup
    free(game);
    return 0;
}
```

### Rendering the Grid

```c
void renderGame(GameState *game) {
    // Render locked pieces
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (game->grid[y][x] != 0) {
                drawCell(x, y, COLOR_BLUE);  // Locked piece
            }
        }
    }
    
    // Render current piece
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (game->currentPiece.shape[py][px] != 0) {
                int gridX = game->currentPiece.x + px;
                int gridY = game->currentPiece.y + py;
                drawCell(gridX, gridY, COLOR_RED);  // Active piece
            }
        }
    }
}
```

### Ghost Piece Preview

```c
// Show where piece will land
Tetromino ghostPiece = game->currentPiece;

// Drop until collision
while (simulateMove(game, ghostPiece, DOWN) == 0) {
    ghostPiece.y += 1;
}

// Render ghost piece with transparency
renderPiece(ghostPiece, COLOR_GRAY_TRANSPARENT);
```

## Implementation Notes

### Design Decisions

1. **Value-based Tetromino**: The `rotatePiece()` function returns a new `Tetromino` by value rather than modifying in-place. This allows safe "try before apply" logic.

2. **Separate Simulation**: The `simulateMove()` function doesn't modify state, enabling validation before application.

3. **Simple Grid Representation**: The grid uses integers (0/1) rather than color/piece-type information. Add a parallel array if you need piece tracking.

4. **Anti-clockwise Rotation**: All rotations are 90° counter-clockwise. For clockwise, call `rotatePiece()` three times.

### Known Limitations

1. **No Wall Kicks**: Rotation fails if the result collides. Advanced Tetris implements "wall kicks" to allow rotation near walls.

2. **No Piece Preview**: The game doesn't maintain a "next piece" queue. You can extend `GameState` to add this.

3. **No Hold Mechanic**: Modern Tetris allows holding a piece. Not implemented here.

4. **No Scoring System**: The engine only returns lines cleared. Scoring is left to the application layer.

### Performance Characteristics

All operations are O(1) or O(GRID_SIZE):

- `createTetromino()`: O(1)
- `rotatePiece()`: O(1) - fixed 4×4 iteration
- `checkPieceCollisionWithGrid()`: O(1) - fixed 4×4 iteration
- `lockPieceInGrid()`: O(1)
- `clearLines()`: O(GRID_WIDTH × GRID_HEIGHT) = O(200)

### Thread Safety

**Not thread-safe.** All functions assume single-threaded access. If using multiple threads:
- Protect `GameState` with a mutex
- Don't share `GameState*` across threads without synchronization

### Memory Management

The API uses manual memory management:
- `gameState_new()` allocates memory (must be freed)
- All other functions operate on stack or caller-provided memory
- No hidden allocations

```c
// Correct usage
GameState *game = gameState_new();
// ... use game ...
free(game);  // Don't forget!
```

### Extension Points

To add features:

1. **Scoring**: Track cleared lines in `GameState`, add score field
2. **Levels**: Add level and speed fields, adjust drop rate
3. **Colors**: Replace `int grid[GRID_HEIGHT][GRID_WIDTH]` with struct array storing piece type
4. **Next Piece**: Add `Tetromino nextPiece` field to `GameState`
5. **Hold Mechanic**: Add `Tetromino heldPiece` and `bool canHold` fields

### Testing Checklist

When testing the engine:
- ✓ Piece spawns at correct position
- ✓ All 7 pieces rotate correctly
- ✓ Collision detection works at boundaries
- ✓ Pieces lock when hitting bottom/other pieces
- ✓ Lines clear correctly (including multiple simultaneous lines)
- ✓ Game over triggers when piece can't spawn
- ✓ Grid shifts down properly after line clear

---

**Last Updated:** 2025-01-01  
**API Version:** 1.0  
**Author:** Utsav Nandi
