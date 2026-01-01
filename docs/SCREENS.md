# Screens / Components

This project uses the standard raylib “screen” pattern: each screen is a self-contained component with a lifecycle (`Init`, `Update`, `Draw`, `Unload`, `Finish`).

Public declarations live in `src/screens.h`; implementations are in `src/screen_*.c`.

---

## Screen lifecycle contract

For each screen:

- **`Init<Name>Screen()`**: Initialize per-screen state (allocate memory, reset counters, etc.)
- **`Update<Name>Screen()`**: Process input + update simulation (one frame)
- **`Draw<Name>Screen()`**: Render (one frame)
- **`Unload<Name>Screen()`**: Free per-screen state/resources
- **`Finish<Name>Screen()`**: Return non-zero to request a transition

The screen manager in `src/raylib_game.c` calls `Update…` + `Draw…` every frame and uses `Finish…` to decide when to transition.

---

## Current screens

### `LOGO` (`src/screen_logo.c`)

- **Purpose**: Shows the default raylib animated logo sequence.
- **Transition condition**: `FinishLogoScreen()` returns `1` after the animation + fade completes.

### `TITLE` (`src/screen_title.c`)

- **Purpose**: Placeholder title screen.
- **Inputs**:
  - `Enter` or `Tap`: request transition to gameplay
- **Transition condition**:
  - `FinishTitleScreen()` returns `2` to request `GAMEPLAY` (see `src/raylib_game.c` switch logic).

### `OPTIONS` (`src/screen_options.c`)

- **Purpose**: Placeholder options screen (currently empty).
- **Notes**: `UpdateOptionsScreen()`/`DrawOptionsScreen()` are TODO stubs and `finishScreen` is never set, so it won’t naturally transition unless you add logic.

### `GAMEPLAY` (`src/screen_gameplay.c`)

- **Purpose**: Runs the Tetris gameplay loop and renders the grid/pieces.
- **Dependencies**: Uses the public Tetris engine API from `src/tetris.h`.
- **Inputs**:
  - `Left` / swipe-left: move piece left (if valid)
  - `Right` / swipe-right: move piece right (if valid)
  - `Up` / swipe-up: rotate piece (if valid)
  - `Down` / swipe-down: move piece down by one cell if possible; otherwise lock, clear lines, spawn next piece
  - `Escape`: end gameplay (transitions to `ENDING`)
- **Sound**:
  - Plays `fxCoin` when clearing one or more lines
  - Plays `fxCoin` on escape-to-finish
- **Transition condition**:
  - `FinishGameplayScreen()` returns `1` when the screen requests exit (escape pressed or game over).

### `ENDING` (`src/screen_ending.c`)

- **Purpose**: Placeholder ending screen.
- **Inputs**:
  - `Enter` or `Tap`: request transition to `TITLE`
- **Transition condition**:
  - `FinishEndingScreen()` returns `1`.

