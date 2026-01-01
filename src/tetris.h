#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdlib.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

//----------------------------------------------------------------------------------
// Public types
//----------------------------------------------------------------------------------

typedef enum MOVE {
    ROTATE,
    DOWN,
    LEFT,
    RIGHT
} MOVE;

typedef enum GameStatus {
    TO_START,
    RUNNING,
    GAME_OVER
} GameStatus;

typedef struct {
    int shape[4][4]; 
} TetrominoDef;

typedef struct {
    int shape[4][4]; 
    int x, y;
} Tetromino;

typedef struct {
    int grid[GRID_HEIGHT][GRID_WIDTH];
    Tetromino currentPiece;
    GameStatus isGameRunning;
} GameState;

//----------------------------------------------------------------------------------
// Public tetromino definitions
//----------------------------------------------------------------------------------

extern const TetrominoDef TETROMINO_SQR;
extern const TetrominoDef TETROMINO_LINE;
extern const TetrominoDef TETROMINO_Z;
extern const TetrominoDef TETROMINO_RZ;
extern const TetrominoDef TETROMINO_T;
extern const TetrominoDef TETROMINO_L;
extern const TetrominoDef TETROMINO_RL;
extern const TetrominoDef TETROMINOS[7];

//----------------------------------------------------------------------------------
// Public API
//----------------------------------------------------------------------------------

// Allocates a new GameState (heap). Caller must free().
GameState *gameState_new(void);

// Resets the grid, sets RUNNING, and spawns the first piece.
void startNewGame(GameState *state);

// Creates a piece instance from a definition at a given grid position.
Tetromino createTetromino(const TetrominoDef *def, int startX, int startY);

// Returns an anti-clockwise rotated copy of the given piece.
Tetromino rotatePiece(Tetromino piece);

// Applies a move to a copy of 'piece' and checks collisions against 'state'.
// Returns:
// - For LEFT/RIGHT/ROTATE: 0 valid, -1 out-of-bounds, -2 locked-piece collision.
// - For DOWN: 0 valid, 1 cannot move down (hit bottom or locked-piece collision).
int simulateMove(GameState *state, Tetromino piece, MOVE currentMove);

// Returns 0 valid, -1 out-of-bounds, -2 locked-piece collision.
int checkPieceCollisionWithGrid(GameState *state, Tetromino piece);

// Writes occupied piece cells into state->grid (marks as 1).
void lockPieceInGrid(GameState *state, Tetromino piece);

// Clears filled lines and compacts the grid; returns number of lines cleared.
int clearLines(GameState *state);

#endif // TETRIS_H
