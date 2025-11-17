#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdlib.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

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

extern const TetrominoDef TETROMINO_SQR;
extern const TetrominoDef TETROMINO_LINE;
extern const TetrominoDef TETROMINO_Z;
extern const TetrominoDef TETROMINO_RZ;
extern const TetrominoDef TETROMINO_T;
extern const TetrominoDef TETROMINO_L;
extern const TetrominoDef TETROMINO_RL;
extern const TetrominoDef TETROMINOS[7];

GameState *gameState_new(void);
void startNewGame(GameState *state);
Tetromino createTetromino(const TetrominoDef *def, int startX, int startY);
Tetromino rotatePiece(Tetromino piece);
int simulateMove(GameState *state, Tetromino piece, MOVE currentMove);
int checkPieceCollisionWithGrid(GameState *state, Tetromino piece);
void lockPieceInGrid(GameState *state, Tetromino piece);
int clearLines(GameState *state);

#endif // TETRIS_H
