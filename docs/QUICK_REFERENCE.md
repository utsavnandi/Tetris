# Tetris API Quick Reference

A concise reference card for the most commonly used APIs and patterns.

## 🎮 Tetris Engine Essentials

### Game State Management

```c
// Create and initialize
GameState *game = gameState_new();
startNewGame(game);

// Check game status
if (game->isGameRunning == RUNNING) { /* ... */ }
if (game->isGameRunning == GAME_OVER) { /* ... */ }

// Cleanup
free(game);
```

### Piece Operations

```c
// Create piece
Tetromino piece = createTetromino(&TETROMINO_T, x, y);

// Standard spawn position
int spawnX = GRID_WIDTH / 2 - 2;  // Center (3)
int spawnY = 0;                    // Top

// Rotate
Tetromino rotated = rotatePiece(piece);

// Move
piece.x += 1;  // Right
piece.x -= 1;  // Left
piece.y += 1;  // Down
```

### Collision & Validation

```c
// Validate before moving
int result = simulateMove(game, game->currentPiece, LEFT);
if (result == 0) {
    // Valid move - apply it
    game->currentPiece.x -= 1;
}

// Check collision directly
int collision = checkPieceCollisionWithGrid(game, piece);
// Returns: 0 (valid), -1 (bounds), -2 (locked piece)
```

### Locking & Line Clearing

```c
// Lock piece permanently
lockPieceInGrid(game, game->currentPiece);

// Clear completed lines
int linesCleared = clearLines(game);

// Spawn next piece
int idx = rand() % 7;
game->currentPiece = createTetromino(&TETROMINOS[idx], spawnX, spawnY);

// Check game over
if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
    game->isGameRunning = GAME_OVER;
}
```

## 🖥️ Screen System Essentials

### Screen Lifecycle Functions

```c
// Every screen must implement these 5 functions:

void InitMyScreen(void) {
    // Initialize screen variables
    framesCounter = 0;
    finishScreen = 0;
}

void UpdateMyScreen(void) {
    // Handle input, update logic
    if (IsKeyPressed(KEY_ENTER)) {
        finishScreen = 1;
    }
}

void DrawMyScreen(void) {
    // Render visuals
    DrawText("Hello", 100, 100, 20, WHITE);
}

void UnloadMyScreen(void) {
    // Cleanup resources
    free(screenData);
}

int FinishMyScreen(void) {
    // Return transition code
    return finishScreen;  // 0=stay, 1+=go to screen
}
```

### Screen Transitions

```c
// Signal transition (in Update function)
finishScreen = 1;  // Will transition when Finish() returns this

// Main loop handles actual transition
if (FinishMyScreen() == 1) {
    TransitionToScreen(NEXT_SCREEN);
}
```

### Global Shared Resources

```c
// Available in all screens (defined in screens.h)
extern GameScreen currentScreen;  // Current screen enum
extern Font font;                  // Shared font
extern Sound fxCoin;               // Shared sound effect
extern Music music;                // Shared music
```

## 🎯 Common Patterns

### Input Handling

```c
// Keyboard
if (IsKeyPressed(KEY_LEFT)) { /* ... */ }
if (IsKeyPressedRepeat(KEY_LEFT)) { /* ... */ }  // With repeat
if (IsKeyDown(KEY_LEFT)) { /* ... */ }           // Held down

// Gestures (mobile)
if (IsGestureDetected(GESTURE_SWIPE_LEFT)) { /* ... */ }
if (IsGestureDetected(GESTURE_TAP)) { /* ... */ }
```

### Frame-based Animation

```c
static int framesCounter = 0;

void UpdateMyScreen(void) {
    framesCounter++;
    
    // Every second (at 60 FPS)
    if (framesCounter % 60 == 0) {
        // Do something periodic
    }
}
```

### State Machine

```c
static int state = 0;

void UpdateMyScreen(void) {
    switch (state) {
        case 0:  // Init state
            if (condition) state = 1;
            break;
        case 1:  // Active state
            if (done) state = 2;
            break;
        case 2:  // Exit state
            finishScreen = 1;
            break;
    }
}
```

### Fading Effects

```c
static float alpha = 1.0f;

void UpdateMyScreen(void) {
    alpha -= 0.02f;  // Fade out
    if (alpha <= 0.0f) finishScreen = 1;
}

void DrawMyScreen(void) {
    DrawText("Fading", 100, 100, 40, Fade(WHITE, alpha));
}
```

## 🔢 Constants & Enums

### Grid Dimensions

```c
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
```

### Move Types

```c
typedef enum MOVE {
    ROTATE,
    DOWN,
    LEFT,
    RIGHT
} MOVE;
```

### Game Status

```c
typedef enum GameStatus {
    TO_START,
    RUNNING,
    GAME_OVER
} GameStatus;
```

### Screen Types

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

## 📦 Data Structures

### GameState

```c
typedef struct {
    int grid[GRID_HEIGHT][GRID_WIDTH];  // Locked pieces
    Tetromino currentPiece;              // Active piece
    GameStatus isGameRunning;            // Game status
} GameState;
```

### Tetromino

```c
typedef struct {
    int shape[4][4];  // 4x4 piece shape
    int x, y;         // Grid position
} Tetromino;
```

### TetrominoDef

```c
typedef struct {
    int shape[4][4];  // Piece template
} TetrominoDef;
```

## 🎨 Piece Definitions

```c
extern const TetrominoDef TETROMINO_SQR;    // Square (O)
extern const TetrominoDef TETROMINO_LINE;   // Line (I)
extern const TetrominoDef TETROMINO_Z;      // Z
extern const TetrominoDef TETROMINO_RZ;     // Reverse Z (S)
extern const TetrominoDef TETROMINO_T;      // T
extern const TetrominoDef TETROMINO_L;      // L
extern const TetrominoDef TETROMINO_RL;     // Reverse L (J)
extern const TetrominoDef TETROMINOS[7];    // Array of all
```

## 🚀 Complete Game Loop Template

```c
#include "tetris.h"

int main(void) {
    // Initialize
    GameState *game = gameState_new();
    startNewGame(game);
    
    // Game loop
    while (game->isGameRunning == RUNNING) {
        // Get input
        MOVE move = /* get player input */;
        
        // Validate move
        if (simulateMove(game, game->currentPiece, move) == 0) {
            // Apply move
            switch (move) {
                case LEFT:  game->currentPiece.x -= 1; break;
                case RIGHT: game->currentPiece.x += 1; break;
                case DOWN:  game->currentPiece.y += 1; break;
                case ROTATE:
                    game->currentPiece = rotatePiece(game->currentPiece);
                    break;
            }
        }
        
        // Gravity (auto-drop)
        if (simulateMove(game, game->currentPiece, DOWN) == 1) {
            lockPieceInGrid(game, game->currentPiece);
            clearLines(game);
            
            // Spawn next
            int idx = rand() % 7;
            game->currentPiece = createTetromino(
                &TETROMINOS[idx], GRID_WIDTH/2-2, 0);
            
            // Check game over
            if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
                game->isGameRunning = GAME_OVER;
            }
        }
        
        // Render
        /* draw grid and pieces */
    }
    
    // Cleanup
    free(game);
    return 0;
}
```

## 🎮 raylib Integration

### Drawing Grid

```c
void drawGrid(GameState *game) {
    const int cellWidth = 40;
    const int cellHeight = 34;
    const int offsetX = 80;
    const int offsetY = 20;
    
    // Draw locked pieces
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (game->grid[y][x] != 0) {
                int screenX = offsetX + x * cellWidth;
                int screenY = offsetY + y * cellHeight;
                DrawRectangle(screenX, screenY, 
                             cellWidth-1, cellHeight-1, BLUE);
            }
        }
    }
}
```

### Drawing Current Piece

```c
void drawCurrentPiece(GameState *game) {
    const int cellWidth = 40;
    const int cellHeight = 34;
    const int offsetX = 80;
    const int offsetY = 20;
    
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (game->currentPiece.shape[py][px] != 0) {
                int gridX = game->currentPiece.x + px;
                int gridY = game->currentPiece.y + py;
                int screenX = offsetX + gridX * cellWidth;
                int screenY = offsetY + gridY * cellHeight;
                DrawRectangle(screenX, screenY, 
                             cellWidth-1, cellHeight-1, RED);
            }
        }
    }
}
```

## 🔧 Return Values Quick Reference

### simulateMove()
- `0` - Move is valid
- `1` - DOWN move hit bottom (should lock)
- `-1` - Out of bounds
- `-2` - Collision with locked piece

### checkPieceCollisionWithGrid()
- `0` - No collision
- `-1` - Out of bounds
- `-2` - Collision with locked piece

### FinishXXXScreen()
- `0` - Stay on current screen
- `1+` - Transition to specific screen (screen-dependent)

### clearLines()
- `0-4` - Number of lines cleared

## 💡 Pro Tips

1. **Always validate before moving:**
   ```c
   if (simulateMove(...) == 0) { /* apply move */ }
   ```

2. **Check game over after spawning:**
   ```c
   if (checkPieceCollisionWithGrid(...) != 0) { /* game over */ }
   ```

3. **Use value-based rotation:**
   ```c
   Tetromino rotated = rotatePiece(piece);  // Returns new piece
   if (valid) piece = rotated;  // Apply only if valid
   ```

4. **Clear lines after locking:**
   ```c
   lockPieceInGrid(game, piece);
   int cleared = clearLines(game);  // Don't forget!
   ```

5. **Standard spawn position:**
   ```c
   int spawnX = GRID_WIDTH / 2 - 2;  // Always centered
   ```

## 📚 See Also

- [Full API Documentation](./API_OVERVIEW.md)
- [Detailed Examples](./EXAMPLES.md)
- [Build Instructions](./BUILD_GUIDE.md)
- [Tetris Engine Reference](./TETRIS_API.md)
- [Screen System Reference](./SCREEN_API.md)

---

**Version:** 1.0  
**Last Updated:** 2025-01-01

*For detailed explanations, see the full documentation.*
