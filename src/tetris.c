/* 
    TODO: Implement
    Minimal Tetris game logic and data structures needed:

    Data structure needed for tetris game state:
        Custom struct storing current game state as a 2D grid - each cell is empty or filled.
        The grid to store locked pieces (already placed)
        Current piece is randomly generated from different tetromino shapes. 
        The current piece stored separately with its own position/rotation
        Merge current piece and grid only when drawing or checking collisions

    Game Logic:
        Handle the concept of collision detection. 
        Handle piece rotation and movement within the grid boundaries.
        Handle line clearing when a row is completely filled.
        Handle game over condition when new pieces cannot be placed.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "tetris.h"


// controls: 
//  up arrow        - anti-clockwise piece rotation
//  down array      - piece falls down by 1 unit for each unit press
//  left and right  - move piece left and right


const TetrominoDef TETROMINO_SQR = {
    .shape = {
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    },
};

const TetrominoDef TETROMINO_LINE = {
    .shape = {
        { 1, 1, 1, 1 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINO_Z = {
    .shape = {
        { 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINO_RZ = {
    .shape = {
        { 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINO_T = {
    .shape = {
        { 0, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINO_L = {
    .shape = {
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINO_RL = {
    .shape = {
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

const TetrominoDef TETROMINOS[7] = {
    TETROMINO_SQR,
    TETROMINO_LINE,
    TETROMINO_Z,
    TETROMINO_RZ,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_RL
};

GameState *gameState_new(void) {
    GameState *s = calloc(1, sizeof *s);
    if (!s) return NULL;
    s->isGameRunning = TO_START;
    return s;
}

void startNewGame(GameState *state) {
    memset(state->grid, 0, sizeof(state->grid));
    state->isGameRunning = RUNNING;
    // generate first piece
    srand(time(NULL));
    int randIndex = rand() % 7;
    state->currentPiece = createTetromino(&TETROMINOS[randIndex], GRID_WIDTH / 2 - 2, 0);
    if(rand() % 2 == 0) {
        state->currentPiece = rotatePiece(state->currentPiece);
    }
}

// void runGameTick(GameState *state, MOVE move) {
// }

Tetromino createTetromino(const TetrominoDef *def, int startX, int startY) {
    Tetromino piece;
    memcpy(piece.shape, def->shape, sizeof(piece.shape));
    piece.x = startX;
    piece.y = startY;
    return piece;
}

// TODO: should be passed by reference to avoid copying
// void rotatePiece(Tetromino *piece) {
Tetromino rotatePiece(Tetromino piece) {
    Tetromino rotated = piece;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) { 
            // always rotated anti-clockwise
            rotated.shape[3 - x][y] = piece.shape[y][x];
        }
    }
    return rotated;
}

// Note: In C we can't pass by refrence directly so we emulate it by passing raw pointers
int simulateMove(GameState *state, Tetromino piece, MOVE currentMove) {
    // simulate a move and check for collision
    switch (currentMove)
    {
    case DOWN:
        piece.y += 1;
        break;
    case LEFT:
        piece.x -= 1;
        break;
    case RIGHT:
        piece.x += 1;
        break;
    case ROTATE:
        piece = rotatePiece(piece);
        break;
    default:
        break;
    }
    // check for collision with locked pieces in the grid
    int result = checkPieceCollisionWithGrid(state, piece);
    if (currentMove == DOWN && result != 0) 
        return 1;
    return result;  

}

// Notes Similar to above func in Notes
int checkPieceCollisionWithGrid(GameState *state, Tetromino piece) {
    // basic idea is to check for:
    //  1. out of bounds -> return -1 : invalid move
    //  2. collision with locked pieces -> return -2 : invalid move
    //  3. valid move -> return 0
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (piece.shape[py][px] == 0) 
                continue; // empty cell in piece
            int gridX = piece.x + px;
            int gridY = piece.y + py;
            if (gridX < 0 || gridX >= GRID_WIDTH || gridY < 0 || gridY >= GRID_HEIGHT) {
                return -1; // invalid move - out of bounds
            }
            if (state->grid[gridY][gridX] != 0) {
                return -2; // invalid move - collision detected with locked piece
            }
        }
    }
    return 0;
}

// TODO: This correct for state but tetromino should be passed by reference as well
void lockPieceInGrid(GameState *state, Tetromino piece) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (piece.shape[py][px] == 0) 
                continue; // empty cell in piece
            int gridX = piece.x + px;
            int gridY = piece.y + py;
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                state->grid[gridY][gridX] = 1; // mark cell as filled
            }
        }
    }
}

int clearLines(GameState *state) {
    int linesCleared = 0;
    
    // Start from bottom, check if row is full, shift rows down as we go
    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool lineFull = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (state->grid[y][x] == 0) {
                lineFull = false;
                break;
            }
        }
        
        if (lineFull) {
            linesCleared++;
        } else if (linesCleared > 0) {
            // Shift this non-full row down by linesCleared positions
            for (int x = 0; x < GRID_WIDTH; x++) {
                state->grid[y + linesCleared][x] = state->grid[y][x];
            }
        }
    }
    
    // Clear the top linesCleared rows - edge case when the grid is almost full
    for (int y = 0; y < linesCleared; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            state->grid[y][x] = 0;
        }
    }
    
    return linesCleared;
}
