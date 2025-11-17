/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "tetris.h"
#include <stdio.h>
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
GameState *game;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    game = gameState_new();
    startNewGame(game);
    TraceLog(LOG_INFO, "Gameplay screen initialized");
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)|| IsGestureDetected(GESTURE_SWIPE_LEFT)) {
        // move piece left
        if (simulateMove(game, game->currentPiece, LEFT) == 0) {
            game->currentPiece.x -= 1;
        }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)|| IsGestureDetected(GESTURE_SWIPE_RIGHT)) {
        // move piece right
        if (simulateMove(game, game->currentPiece, RIGHT) == 0) {
            game->currentPiece.x += 1;
        }
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP) || IsGestureDetected(GESTURE_SWIPE_UP)) {
        // rotate piece
        Tetromino rotatedPiece = rotatePiece(game->currentPiece);
        if (simulateMove(game, rotatedPiece, ROTATE) == 0) {
            game->currentPiece = rotatedPiece;
        }
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN) || IsGestureDetected(GESTURE_SWIPE_DOWN)) {
        // move piece down
        if (simulateMove(game, game->currentPiece, DOWN) == 0) {
            game->currentPiece.y += 1;
        } else {
            // lock piece in place
            lockPieceInGrid(game, game->currentPiece);
            // clear lines
            int linesCleared = clearLines(game);
            if (linesCleared > 0) {
                PlaySound(fxCoin);
            }
            // generate new piece
            int randIndex = rand() % 7;
            game->currentPiece = createTetromino(&TETROMINOS[randIndex], GRID_WIDTH / 2 - 2, 0);
            if (simulateMove(game, game->currentPiece, DOWN) != 0) {
                // game over
                game->isGameRunning = GAME_OVER;
                finishScreen = 1;
            }
        }
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // clear screen
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), LIGHTGRAY);
    // draw side bars
    DrawRectangle(40, 20, 40, 720, GRAY);
    DrawRectangle(80, 700, 400, 40, GRAY);
    DrawRectangle(480, 20, 40, 720, GRAY);
    // draw outer grid
    DrawRectangleLines(80, 20, 400, 680, BLACK);
    // draw grid
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            DrawRectangleLines(
                80 + j * 400 / GRID_WIDTH, 
                20 + i * 680 / GRID_HEIGHT, 
                400 / GRID_WIDTH, 
                680 / GRID_HEIGHT, DARKGRAY
            );
        }
    }
    // draw locked pieces
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (game->grid[i][j] != 0) {
                DrawRectangle(
                    80 + j * 400 / GRID_WIDTH + 1, 
                    20 + i * 680 / GRID_HEIGHT + 1, 
                    400 / GRID_WIDTH - 1, 
                    680 / GRID_HEIGHT - 1, BLUE
                );
            }
        }
    }
    // draw current piece
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (game->currentPiece.shape[py][px] != 0) {
                int gridX = game->currentPiece.x + px;
                int gridY = game->currentPiece.y + py;
                DrawRectangle(
                    80 + gridX * 400 / GRID_WIDTH + 1, 
                    20 + gridY * 680 / GRID_HEIGHT + 1, 
                    400 / GRID_WIDTH - 1, 
                    680 / GRID_HEIGHT - 1, RED
                );
            }
        }
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    free(game);
    TraceLog(LOG_INFO, "Gameplay screen unloaded");
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    // here we simply return 1 or 0 depending on if the screen should finish
    return finishScreen;
}