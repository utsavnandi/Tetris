# Examples and Tutorials

## Table of Contents

1. [Getting Started](#getting-started)
2. [Basic Tutorials](#basic-tutorials)
3. [Advanced Examples](#advanced-examples)
4. [Code Snippets](#code-snippets)
5. [Common Patterns](#common-patterns)
6. [Troubleshooting](#troubleshooting)

## Getting Started

### Hello Tetris - Minimal Example

The simplest way to use the Tetris engine:

```c
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Create and initialize game
    GameState *game = gameState_new();
    startNewGame(game);
    
    printf("Tetris game started!\n");
    printf("Current piece position: (%d, %d)\n", 
           game->currentPiece.x, game->currentPiece.y);
    
    // Cleanup
    free(game);
    return 0;
}
```

**Compile:**
```bash
gcc -o hello_tetris hello_tetris.c tetris.c -I../src
./hello_tetris
```

### Console Tetris - Text-Based Game

A complete playable Tetris in the terminal:

```c
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Clear screen (UNIX/Linux)
void clearScreen() {
    printf("\033[2J\033[H");
}

// Print the game grid
void printGrid(GameState *game) {
    clearScreen();
    
    // Top border
    printf("+");
    for (int x = 0; x < GRID_WIDTH; x++) printf("--");
    printf("+\n");
    
    // Grid with current piece overlay
    for (int y = 0; y < GRID_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < GRID_WIDTH; x++) {
            bool isPiece = false;
            
            // Check if current piece occupies this cell
            for (int py = 0; py < 4; py++) {
                for (int px = 0; px < 4; px++) {
                    if (game->currentPiece.shape[py][px] != 0) {
                        int gridX = game->currentPiece.x + px;
                        int gridY = game->currentPiece.y + py;
                        if (gridX == x && gridY == y) {
                            isPiece = true;
                        }
                    }
                }
            }
            
            if (isPiece) {
                printf("##");  // Active piece
            } else if (game->grid[y][x] != 0) {
                printf("[]");  // Locked piece
            } else {
                printf("  ");  // Empty
            }
        }
        printf("|\n");
    }
    
    // Bottom border
    printf("+");
    for (int x = 0; x < GRID_WIDTH; x++) printf("--");
    printf("+\n");
    
    printf("Controls: a=left, d=right, w=rotate, s=down, q=quit\n");
}

int main(void) {
    GameState *game = gameState_new();
    startNewGame(game);
    
    char input;
    int dropCounter = 0;
    
    while (game->isGameRunning == RUNNING) {
        printGrid(game);
        
        // Auto-drop every 30 frames
        dropCounter++;
        if (dropCounter >= 30) {
            dropCounter = 0;
            
            if (simulateMove(game, game->currentPiece, DOWN) == 0) {
                game->currentPiece.y += 1;
            } else {
                // Lock and spawn new piece
                lockPieceInGrid(game, game->currentPiece);
                int cleared = clearLines(game);
                
                if (cleared > 0) {
                    printf("Cleared %d lines!\n", cleared);
                    usleep(500000);  // Pause for effect
                }
                
                // Spawn new piece
                int randIdx = rand() % 7;
                game->currentPiece = createTetromino(
                    &TETROMINOS[randIdx], GRID_WIDTH/2 - 2, 0);
                
                // Check game over
                if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
                    game->isGameRunning = GAME_OVER;
                }
            }
        }
        
        // Get user input (non-blocking would be better)
        printf("Move: ");
        scanf(" %c", &input);
        
        switch (input) {
            case 'a':  // Left
                if (simulateMove(game, game->currentPiece, LEFT) == 0) {
                    game->currentPiece.x -= 1;
                }
                break;
            case 'd':  // Right
                if (simulateMove(game, game->currentPiece, RIGHT) == 0) {
                    game->currentPiece.x += 1;
                }
                break;
            case 'w':  // Rotate
                {
                    Tetromino rotated = rotatePiece(game->currentPiece);
                    if (checkPieceCollisionWithGrid(game, rotated) == 0) {
                        game->currentPiece = rotated;
                    }
                }
                break;
            case 's':  // Down
                if (simulateMove(game, game->currentPiece, DOWN) == 0) {
                    game->currentPiece.y += 1;
                }
                break;
            case 'q':  // Quit
                game->isGameRunning = GAME_OVER;
                break;
        }
    }
    
    printf("\nGAME OVER!\n");
    free(game);
    return 0;
}
```

## Basic Tutorials

### Tutorial 1: Creating and Moving Pieces

Learn the fundamentals of piece manipulation:

```c
#include "tetris.h"
#include <stdio.h>

void tutorial_piece_basics(void) {
    printf("=== Tetris Piece Basics ===\n\n");
    
    // Create a T-piece
    Tetromino piece = createTetromino(&TETROMINO_T, 3, 0);
    
    printf("Initial position: (%d, %d)\n", piece.x, piece.y);
    
    // Move right
    piece.x += 1;
    printf("After moving right: (%d, %d)\n", piece.x, piece.y);
    
    // Move down
    piece.y += 1;
    printf("After moving down: (%d, %d)\n", piece.x, piece.y);
    
    // Rotate the piece
    Tetromino rotated = rotatePiece(piece);
    printf("Piece rotated successfully\n");
    
    // Print shape before rotation
    printf("\nOriginal shape:\n");
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%c ", piece.shape[y][x] ? '#' : '.');
        }
        printf("\n");
    }
    
    // Print shape after rotation
    printf("\nRotated shape:\n");
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%c ", rotated.shape[y][x] ? '#' : '.');
        }
        printf("\n");
    }
}

int main(void) {
    tutorial_piece_basics();
    return 0;
}
```

**Output:**
```
=== Tetris Piece Basics ===

Initial position: (3, 0)
After moving right: (4, 0)
After moving down: (4, 1)
Piece rotated successfully

Original shape:
. # . . 
# # # . 
. . . . 
. . . . 

Rotated shape:
. # . . 
. # # . 
. # . . 
. . . . 
```

### Tutorial 2: Collision Detection

Understand how collision detection works:

```c
#include "tetris.h"
#include <stdio.h>

void tutorial_collision(void) {
    printf("=== Collision Detection Tutorial ===\n\n");
    
    GameState *game = gameState_new();
    startNewGame(game);
    
    // Try to move piece to the left boundary
    printf("Testing boundary collision...\n");
    game->currentPiece.x = 0;  // Move to left edge
    
    if (simulateMove(game, game->currentPiece, LEFT) != 0) {
        printf("✓ Correctly detected left boundary\n");
    }
    
    // Try to move piece to the right boundary
    game->currentPiece.x = GRID_WIDTH - 1;  // Right edge
    
    if (simulateMove(game, game->currentPiece, RIGHT) != 0) {
        printf("✓ Correctly detected right boundary\n");
    }
    
    // Create a locked piece at the bottom
    printf("\nTesting collision with locked pieces...\n");
    game->currentPiece.y = GRID_HEIGHT - 5;
    
    // Lock a piece below
    for (int x = 0; x < GRID_WIDTH; x++) {
        game->grid[GRID_HEIGHT - 1][x] = 1;
    }
    
    // Try to move down onto locked piece
    for (int i = 0; i < 10; i++) {
        if (simulateMove(game, game->currentPiece, DOWN) == 0) {
            game->currentPiece.y += 1;
            printf("Moved down to y=%d\n", game->currentPiece.y);
        } else {
            printf("✓ Detected collision at y=%d\n", game->currentPiece.y);
            break;
        }
    }
    
    free(game);
}

int main(void) {
    tutorial_collision();
    return 0;
}
```

### Tutorial 3: Line Clearing

Learn how line clearing works:

```c
#include "tetris.h"
#include <stdio.h>

void printGridState(GameState *game) {
    for (int y = GRID_HEIGHT - 5; y < GRID_HEIGHT; y++) {
        printf("Row %2d: ", y);
        for (int x = 0; x < GRID_WIDTH; x++) {
            printf("%c ", game->grid[y][x] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

void tutorial_line_clearing(void) {
    printf("=== Line Clearing Tutorial ===\n\n");
    
    GameState *game = gameState_new();
    memset(game->grid, 0, sizeof(game->grid));
    
    // Create some complete lines
    printf("Setting up test grid with 2 complete lines...\n\n");
    
    // Bottom row: complete
    for (int x = 0; x < GRID_WIDTH; x++) {
        game->grid[GRID_HEIGHT - 1][x] = 1;
    }
    
    // Second from bottom: incomplete (missing 1 cell)
    for (int x = 0; x < GRID_WIDTH - 1; x++) {
        game->grid[GRID_HEIGHT - 2][x] = 1;
    }
    
    // Third from bottom: complete
    for (int x = 0; x < GRID_WIDTH; x++) {
        game->grid[GRID_HEIGHT - 3][x] = 1;
    }
    
    // Fourth from bottom: empty
    
    printf("Before clearing:\n");
    printGridState(game);
    
    int cleared = clearLines(game);
    
    printf("After clearing:\n");
    printGridState(game);
    
    printf("Total lines cleared: %d\n", cleared);
    
    free(game);
}

int main(void) {
    tutorial_line_clearing();
    return 0;
}
```

**Output:**
```
=== Line Clearing Tutorial ===

Setting up test grid with 2 complete lines...

Before clearing:
Row 15: . . . . . . . . . . 
Row 16: . . . . . . . . . . 
Row 17: # # # # # # # # # # 
Row 18: # # # # # # # # # . 
Row 19: # # # # # # # # # # 

After clearing:
Row 15: . . . . . . . . . . 
Row 16: . . . . . . . . . . 
Row 17: . . . . . . . . . . 
Row 18: . . . . . . . . . . 
Row 19: # # # # # # # # # . 

Total lines cleared: 2
```

## Advanced Examples

### Example 1: Ghost Piece (Preview)

Show where the piece will land:

```c
#include "tetris.h"
#include "raylib.h"

void drawGhostPiece(GameState *game) {
    // Create a copy of current piece
    Tetromino ghost = game->currentPiece;
    
    // Drop it until collision
    while (simulateMove(game, ghost, DOWN) == 0) {
        ghost.y += 1;
    }
    
    // Render ghost piece with transparency
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (ghost.shape[py][px] != 0) {
                int gridX = ghost.x + px;
                int gridY = ghost.y + py;
                
                int screenX = 80 + gridX * 40;
                int screenY = 20 + gridY * 34;
                
                DrawRectangle(screenX, screenY, 39, 33, 
                             Fade(GRAY, 0.3f));  // Semi-transparent
            }
        }
    }
}

// Use in DrawGameplayScreen():
void DrawGameplayScreen(void) {
    // ... draw grid and locked pieces ...
    
    drawGhostPiece(game);      // Draw ghost first (behind)
    drawCurrentPiece(game);     // Draw current piece on top
}
```

### Example 2: Hard Drop

Instantly drop piece to bottom:

```c
void hardDrop(GameState *game) {
    // Drop until collision
    while (simulateMove(game, game->currentPiece, DOWN) == 0) {
        game->currentPiece.y += 1;
    }
    
    // Lock immediately
    lockPieceInGrid(game, game->currentPiece);
    
    // Clear lines
    int linesCleared = clearLines(game);
    
    // Spawn next piece
    int randIdx = rand() % 7;
    game->currentPiece = createTetromino(
        &TETROMINOS[randIdx], GRID_WIDTH/2 - 2, 0);
    
    // Check game over
    if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
        game->isGameRunning = GAME_OVER;
    }
}

// In UpdateGameplayScreen():
if (IsKeyPressed(KEY_SPACE)) {
    hardDrop(game);
    PlaySound(fxHardDrop);
}
```

### Example 3: Hold Piece Mechanic

Allow player to hold a piece for later:

```c
// Add to GameState or as global:
typedef struct {
    GameState *game;
    Tetromino heldPiece;
    bool hasHeldPiece;
    bool canHold;  // Prevent holding multiple times per piece
} ExtendedGameState;

void holdPiece(ExtendedGameState *state) {
    if (!state->canHold) return;  // Already held this turn
    
    if (!state->hasHeldPiece) {
        // First hold - save current piece, spawn new one
        state->heldPiece = state->game->currentPiece;
        state->hasHeldPiece = true;
        
        int randIdx = rand() % 7;
        state->game->currentPiece = createTetromino(
            &TETROMINOS[randIdx], GRID_WIDTH/2 - 2, 0);
    } else {
        // Swap current with held
        Tetromino temp = state->game->currentPiece;
        state->game->currentPiece = state->heldPiece;
        state->heldPiece = temp;
        
        // Reset position
        state->game->currentPiece.x = GRID_WIDTH/2 - 2;
        state->game->currentPiece.y = 0;
    }
    
    state->canHold = false;  // Can't hold again until next piece
}

// Reset when new piece spawns:
void spawnNewPiece(ExtendedGameState *state) {
    // ... normal spawn code ...
    state->canHold = true;  // Allow holding again
}

// Draw held piece:
void drawHeldPiece(ExtendedGameState *state) {
    if (!state->hasHeldPiece) return;
    
    DrawText("HOLD", 550, 50, 20, WHITE);
    
    // Draw held piece preview
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (state->heldPiece.shape[y][x] != 0) {
                DrawRectangle(550 + x*20, 80 + y*20, 18, 18, BLUE);
            }
        }
    }
}

// In UpdateGameplayScreen():
if (IsKeyPressed(KEY_C)) {
    holdPiece(extendedState);
}
```

### Example 4: Piece Preview Queue

Show next 3 pieces:

```c
typedef struct {
    GameState *game;
    Tetromino nextPieces[3];
} GameStateWithPreview;

void initPieceQueue(GameStateWithPreview *state) {
    for (int i = 0; i < 3; i++) {
        int randIdx = rand() % 7;
        state->nextPieces[i] = createTetromino(
            &TETROMINOS[randIdx], 0, 0);
    }
}

void spawnNextPiece(GameStateWithPreview *state) {
    // Use first piece in queue
    state->game->currentPiece = state->nextPieces[0];
    state->game->currentPiece.x = GRID_WIDTH/2 - 2;
    state->game->currentPiece.y = 0;
    
    // Shift queue
    state->nextPieces[0] = state->nextPieces[1];
    state->nextPieces[1] = state->nextPieces[2];
    
    // Generate new piece for end of queue
    int randIdx = rand() % 7;
    state->nextPieces[2] = createTetromino(&TETROMINOS[randIdx], 0, 0);
}

void drawPieceQueue(GameStateWithPreview *state) {
    DrawText("NEXT", 550, 150, 20, WHITE);
    
    for (int i = 0; i < 3; i++) {
        int offsetY = 180 + i * 80;
        
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (state->nextPieces[i].shape[y][x] != 0) {
                    DrawRectangle(550 + x*15, offsetY + y*15, 
                                 13, 13, LIGHTGRAY);
                }
            }
        }
    }
}
```

### Example 5: Scoring System

Implement score tracking:

```c
typedef struct {
    int score;
    int level;
    int totalLines;
    float dropSpeed;  // Seconds per drop
} ScoreState;

void initScoring(ScoreState *state) {
    state->score = 0;
    state->level = 1;
    state->totalLines = 0;
    state->dropSpeed = 1.0f;  // 1 second per drop at level 1
}

int calculateScore(int linesCleared, int level) {
    const int baseScores[] = { 0, 100, 300, 500, 800 };  // 0-4 lines
    
    if (linesCleared < 0 || linesCleared > 4) return 0;
    
    return baseScores[linesCleared] * level;
}

void updateScore(ScoreState *state, int linesCleared) {
    state->totalLines += linesCleared;
    state->score += calculateScore(linesCleared, state->level);
    
    // Level up every 10 lines
    int newLevel = (state->totalLines / 10) + 1;
    if (newLevel > state->level) {
        state->level = newLevel;
        state->dropSpeed *= 0.9f;  // 10% faster each level
        PlaySound(fxLevelUp);
    }
}

// Usage:
int cleared = clearLines(game);
if (cleared > 0) {
    updateScore(&scoreState, cleared);
    
    if (cleared == 4) {
        DrawText("TETRIS!", 250, 300, 40, GOLD);
    }
}

// Draw score:
void drawScore(ScoreState *state) {
    DrawText(TextFormat("Score: %d", state->score), 550, 400, 20, WHITE);
    DrawText(TextFormat("Level: %d", state->level), 550, 430, 20, WHITE);
    DrawText(TextFormat("Lines: %d", state->totalLines), 550, 460, 20, WHITE);
}
```

## Code Snippets

### Snippet: All 7 Pieces Iteration

```c
// Iterate through all tetromino types
for (int i = 0; i < 7; i++) {
    Tetromino piece = createTetromino(&TETROMINOS[i], 0, 0);
    printf("Piece %d:\n", i);
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%c ", piece.shape[y][x] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}
```

### Snippet: Save/Load Game State

```c
#include <stdio.h>

void saveGame(GameState *game, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    fwrite(game, sizeof(GameState), 1, f);
    fclose(f);
}

bool loadGame(GameState *game, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return false;
    
    fread(game, sizeof(GameState), 1, f);
    fclose(f);
    return true;
}

// Usage:
if (IsKeyPressed(KEY_F5)) {
    saveGame(game, "savegame.dat");
    ShowNotification("Game saved!");
}

if (IsKeyPressed(KEY_F9)) {
    if (loadGame(game, "savegame.dat")) {
        ShowNotification("Game loaded!");
    }
}
```

### Snippet: Color-Coded Pieces

```c
Color getPieceColor(const TetrominoDef *def) {
    if (def == &TETROMINO_SQR) return YELLOW;
    if (def == &TETROMINO_LINE) return SKYBLUE;
    if (def == &TETROMINO_Z) return RED;
    if (def == &TETROMINO_RZ) return GREEN;
    if (def == &TETROMINO_T) return PURPLE;
    if (def == &TETROMINO_L) return ORANGE;
    if (def == &TETROMINO_RL) return BLUE;
    return WHITE;
}

// Extend GameState to track piece types:
typedef struct {
    int grid[GRID_HEIGHT][GRID_WIDTH];
    int gridColors[GRID_HEIGHT][GRID_WIDTH];  // Store color index
    Tetromino currentPiece;
    int currentColorIndex;
    // ...
} ColorfulGameState;

void lockPieceWithColor(ColorfulGameState *state) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (state->currentPiece.shape[py][px] != 0) {
                int gridX = state->currentPiece.x + px;
                int gridY = state->currentPiece.y + py;
                
                if (gridX >= 0 && gridX < GRID_WIDTH && 
                    gridY >= 0 && gridY < GRID_HEIGHT) {
                    state->grid[gridY][gridX] = 1;
                    state->gridColors[gridY][gridX] = state->currentColorIndex;
                }
            }
        }
    }
}
```

### Snippet: Particle Effects on Line Clear

```c
#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
} Particle;

Particle particles[200];
int particleCount = 0;

void spawnLineParticles(int lineY) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int i = 0; i < 3; i++) {  // 3 particles per cell
            Particle p;
            p.position.x = 80 + x * 40 + GetRandomValue(-10, 10);
            p.position.y = 20 + lineY * 34 + GetRandomValue(-10, 10);
            p.velocity.x = GetRandomValue(-100, 100) / 50.0f;
            p.velocity.y = GetRandomValue(-100, 0) / 50.0f;
            p.color = (Color){ 255, 255, 100, 255 };
            p.life = 1.0f;
            
            if (particleCount < 200) {
                particles[particleCount++] = p;
            }
        }
    }
}

void updateParticles(float deltaTime) {
    for (int i = 0; i < particleCount; i++) {
        particles[i].position.x += particles[i].velocity.x;
        particles[i].position.y += particles[i].velocity.y;
        particles[i].velocity.y += 0.5f;  // Gravity
        particles[i].life -= deltaTime;
        
        // Remove dead particles
        if (particles[i].life <= 0) {
            particles[i] = particles[--particleCount];
            i--;
        }
    }
}

void drawParticles(void) {
    for (int i = 0; i < particleCount; i++) {
        Color c = particles[i].color;
        c.a = (unsigned char)(255 * particles[i].life);
        DrawCircleV(particles[i].position, 3, c);
    }
}

// When clearing lines:
int linesCleared = clearLines(game);
for (int y = 0; y < GRID_HEIGHT; y++) {
    bool isEmpty = true;
    for (int x = 0; x < GRID_WIDTH; x++) {
        if (game->grid[y][x] != 0) isEmpty = false;
    }
    // If this row was just cleared (now empty but neighbors full)
    // spawnLineParticles(y);
}
```

## Common Patterns

### Pattern: Input Handling with Repeat

```c
typedef struct {
    float holdTime;
    float repeatDelay;
    float repeatRate;
} InputState;

void updateInputWithRepeat(GameState *game, InputState *input) {
    float deltaTime = GetFrameTime();
    
    if (IsKeyDown(KEY_LEFT)) {
        input->holdTime += deltaTime;
        
        bool shouldMove = IsKeyPressed(KEY_LEFT) || 
                         (input->holdTime > input->repeatDelay);
        
        if (shouldMove && simulateMove(game, game->currentPiece, LEFT) == 0) {
            game->currentPiece.x -= 1;
            if (input->holdTime > input->repeatDelay) {
                input->holdTime -= input->repeatRate;
            }
        }
    } else {
        input->holdTime = 0;
    }
}
```

### Pattern: Gravity with Accumulation

```c
float gravityTimer = 0.0f;
float gravitySpeed = 1.0f;  // Seconds per drop

void updateGravity(GameState *game, float deltaTime) {
    gravityTimer += deltaTime;
    
    if (gravityTimer >= gravitySpeed) {
        gravityTimer -= gravitySpeed;
        
        if (simulateMove(game, game->currentPiece, DOWN) == 0) {
            game->currentPiece.y += 1;
        } else {
            lockPieceInGrid(game, game->currentPiece);
            // ... spawn next piece ...
        }
    }
}
```

## Troubleshooting

### Issue: Piece Spawns Out of Bounds

**Problem:** New pieces spawn in invalid positions.

**Solution:**
```c
// Always spawn at top center
int spawnX = GRID_WIDTH / 2 - 2;  // Center (accounts for 4-wide piece)
int spawnY = 0;                    // Top row

game->currentPiece = createTetromino(&TETROMINOS[idx], spawnX, spawnY);
```

### Issue: Rotation Near Walls Fails

**Problem:** Pieces can't rotate near boundaries.

**Solution:** Implement wall kicks (advanced):
```c
Tetromino tryRotateWithKicks(GameState *game, Tetromino piece) {
    Tetromino rotated = rotatePiece(piece);
    
    // Try rotation at current position
    if (checkPieceCollisionWithGrid(game, rotated) == 0) {
        return rotated;
    }
    
    // Try kicking left
    rotated.x -= 1;
    if (checkPieceCollisionWithGrid(game, rotated) == 0) {
        return rotated;
    }
    
    // Try kicking right
    rotated.x += 2;
    if (checkPieceCollisionWithGrid(game, rotated) == 0) {
        return rotated;
    }
    
    // Rotation failed
    return piece;  // Return original
}
```

### Issue: Lines Don't Clear Properly

**Problem:** Completed lines remain on screen.

**Solution:** Ensure you call `clearLines()` after locking:
```c
lockPieceInGrid(game, game->currentPiece);
int cleared = clearLines(game);  // Must call this!

// Optional: visual feedback
if (cleared > 0) {
    printf("Cleared %d lines\n", cleared);
}
```

### Issue: Game Over Not Detected

**Problem:** Game continues even when grid is full.

**Solution:** Check collision immediately after spawning:
```c
// Spawn new piece
game->currentPiece = createTetromino(&TETROMINOS[idx], x, y);

// Check if spawn position is valid
if (checkPieceCollisionWithGrid(game, game->currentPiece) != 0) {
    game->isGameRunning = GAME_OVER;
    // Trigger game over screen
}
```

---

**Last Updated:** 2025-01-01  
**Author:** Utsav Nandi
