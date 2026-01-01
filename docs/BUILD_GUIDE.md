# Build and Setup Guide

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Building the Game](#building-the-game)
3. [Platform-Specific Instructions](#platform-specific-instructions)
4. [Build Configuration](#build-configuration)
5. [IDE Integration](#ide-integration)
6. [Troubleshooting Build Issues](#troubleshooting-build-issues)
7. [Development Setup](#development-setup)
8. [Deployment](#deployment)

## Prerequisites

### Required Tools

- **CMake** (version 3.10 or higher)
- **Ninja** (recommended) or **Make**
- **C Compiler**:
  - GCC 7+ (Linux)
  - Clang 6+ (macOS)
  - MSVC 2019+ or MinGW (Windows)
- **raylib** (automatically downloaded by CMake)

### System Requirements

**Minimum:**
- OS: Windows 7, macOS 10.12, Linux (any modern distro)
- RAM: 512 MB
- Disk: 50 MB free space
- Graphics: OpenGL 3.3 support

**Recommended:**
- OS: Windows 10/11, macOS 12+, Ubuntu 20.04+
- RAM: 2 GB
- Disk: 100 MB free space
- Graphics: Any GPU from last 10 years

## Building the Game

### Quick Start (Linux/macOS)

```bash
# Clone the repository (if not already done)
cd /workspace

# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
cmake -G Ninja ..

# Build
ninja

# Run the game
./raylib_game
```

### Quick Start (Windows)

**Option 1: Command Line (with MinGW)**
```cmd
cd C:\workspace

mkdir build
cd build

cmake -G "MinGW Makefiles" ..
mingw32-make

raylib_game.exe
```

**Option 2: Visual Studio**
```cmd
cd C:\workspace

mkdir build
cd build

cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release

Release\raylib_game.exe
```

### Step-by-Step Build Instructions

#### 1. Create Build Directory

```bash
cd /workspace
mkdir -p build
cd build
```

**Why?** Separates build artifacts from source code (out-of-source build).

#### 2. Configure with CMake

```bash
cmake -G Ninja ..
```

**Common Generators:**
- `Ninja` - Fast, cross-platform (recommended)
- `Unix Makefiles` - Standard on Linux/macOS
- `Visual Studio 17 2022` - Windows with VS 2022
- `Xcode` - macOS with Xcode

**Configuration Options:**
```bash
# Debug build (with symbols)
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized)
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..

# Specify custom compiler
cmake -G Ninja -DCMAKE_C_COMPILER=clang ..
```

#### 3. Build

```bash
# With Ninja
ninja

# With Make
make

# With CMake (generator-agnostic)
cmake --build .

# Parallel build (faster)
cmake --build . -j 8
```

#### 4. Run

```bash
./raylib_game
```

### Clean Build

To rebuild from scratch:

```bash
cd build
rm -rf *           # Delete all build files
cmake -G Ninja ..
ninja
```

Or simply:

```bash
rm -rf build
mkdir build
cd build
cmake -G Ninja ..
ninja
```

## Platform-Specific Instructions

### Linux

#### Ubuntu/Debian

**Install Dependencies:**
```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build \
                     git libasound2-dev mesa-common-dev \
                     libx11-dev libxrandr-dev libxi-dev \
                     libgl1-mesa-dev libglu1-mesa-dev
```

**Build:**
```bash
cd /workspace
mkdir -p build && cd build
cmake -G Ninja ..
ninja
./raylib_game
```

#### Fedora/RHEL

**Install Dependencies:**
```bash
sudo dnf install -y gcc cmake ninja-build git \
                     alsa-lib-devel mesa-libGL-devel \
                     libX11-devel libXrandr-devel libXi-devel
```

**Build:** Same as Ubuntu

#### Arch Linux

**Install Dependencies:**
```bash
sudo pacman -S base-devel cmake ninja git \
               alsa-lib mesa libx11 libxrandr libxi
```

**Build:** Same as Ubuntu

---

### macOS

#### Install Dependencies

**Using Homebrew:**
```bash
# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake ninja git
```

**Using Xcode:**
```bash
xcode-select --install
```

#### Build

```bash
cd /workspace
mkdir -p build && cd build
cmake -G Ninja ..
ninja
./raylib_game
```

#### Create App Bundle (Optional)

```bash
# Build as macOS app
cmake -G Xcode ..
xcodebuild -project raylib_game.xcodeproj -configuration Release

# Run app
open Release/raylib_game.app
```

---

### Windows

#### Option 1: MinGW (Recommended for Beginners)

**Install MinGW:**
1. Download from: https://sourceforge.net/projects/mingw-w64/
2. Install to `C:\mingw-w64`
3. Add to PATH: `C:\mingw-w64\bin`

**Install CMake:**
1. Download from: https://cmake.org/download/
2. Run installer, select "Add to PATH"

**Build:**
```cmd
cd C:\workspace
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
raylib_game.exe
```

#### Option 2: Visual Studio

**Install:**
1. Visual Studio 2019 or 2022 (Community Edition is free)
2. Select "Desktop development with C++" workload
3. Include CMake tools

**Build (Command Line):**
```cmd
cd C:\workspace
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
Release\raylib_game.exe
```

**Build (Visual Studio GUI):**
1. Open Visual Studio
2. File → Open → CMake...
3. Select `C:\workspace\CMakeLists.txt`
4. Build → Build All
5. Run with F5

#### Option 3: MSYS2

**Install MSYS2:**
```bash
# Download from https://www.msys2.org/

# In MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-ninja git
```

**Build:**
```bash
cd /c/workspace
mkdir build && cd build
cmake -G Ninja ..
ninja
./raylib_game.exe
```

---

### Web (Emscripten)

Build the game to run in a web browser.

#### Install Emscripten

```bash
# Clone Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install latest
./emsdk install latest
./emsdk activate latest

# Set environment
source ./emsdk_env.sh  # Linux/macOS
# OR
emsdk_env.bat          # Windows
```

#### Build for Web

```bash
cd /workspace
mkdir -p build-web && cd build-web

# Configure with Emscripten
emcmake cmake -G Ninja ..

# Build
ninja

# Result: raylib_game.html, raylib_game.js, raylib_game.wasm
```

#### Test Locally

```bash
# Start local server
python3 -m http.server 8000

# Open browser to:
# http://localhost:8000/raylib_game.html
```

#### Deploy

Upload these files to your web server:
- `raylib_game.html`
- `raylib_game.js`
- `raylib_game.wasm`
- `raylib_game.data` (if resources embedded)

---

### Android (Advanced)

Requires Android NDK and raylib Android toolchain.

**Prerequisites:**
- Android NDK r21+
- Android SDK with API level 29+

**Build:**
```bash
# Set environment
export ANDROID_NDK=/path/to/ndk

# Use Android Makefile
cd /workspace/src
make -f Makefile.Android

# Result: APK in build folder
```

See raylib documentation for detailed Android build instructions.

## Build Configuration

### CMake Options

Common options to customize the build:

```bash
# Build type
-DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo|MinSizeRel

# Installation prefix
-DCMAKE_INSTALL_PREFIX=/usr/local

# Compiler selection
-DCMAKE_C_COMPILER=gcc
-DCMAKE_CXX_COMPILER=g++

# Force 32-bit build
-DCMAKE_C_FLAGS=-m32

# Enable/disable warnings
-DCMAKE_C_FLAGS="-Wall -Wextra"
```

**Example:**
```bash
cmake -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_C_FLAGS="-O3 -march=native" \
      ..
```

### Build Types

| Type | Optimization | Debug Symbols | Use Case |
|------|-------------|---------------|----------|
| Debug | None (-O0) | Yes | Development |
| Release | Full (-O3) | No | Distribution |
| RelWithDebInfo | Full (-O2) | Yes | Profiling |
| MinSizeRel | Size (-Os) | No | Embedded systems |

**Select Build Type:**
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Compiler Flags

**Debug Build with Sanitizers:**
```bash
cmake -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_FLAGS="-fsanitize=address -fsanitize=undefined" \
      ..
```

**Optimized Build:**
```bash
cmake -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-O3 -march=native -flto" \
      ..
```

## IDE Integration

### Visual Studio Code

**Install Extensions:**
- C/C++ (Microsoft)
- CMake Tools (Microsoft)
- CMake (twxs)

**Configuration (`.vscode/settings.json`):**
```json
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.generator": "Ninja",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

**Build:**
1. Open folder in VS Code
2. CMake: Configure (Ctrl+Shift+P)
3. CMake: Build (F7)
4. Run with debugger (F5)

---

### CLion

**Open Project:**
1. File → Open → Select `/workspace`
2. CLion automatically detects CMakeLists.txt

**Build:**
1. Build → Build Project (Ctrl+F9)
2. Run → Run 'raylib_game' (Shift+F10)
3. Debug with breakpoints (Shift+F9)

**Configuration:**
- Build type: File → Settings → Build, Execution, Deployment → CMake
- Add profiles for Debug/Release

---

### Xcode (macOS)

**Generate Xcode Project:**
```bash
cd /workspace
mkdir build-xcode && cd build-xcode
cmake -G Xcode ..
open raylib_game.xcodeproj
```

**Build in Xcode:**
1. Select scheme: raylib_game
2. Product → Build (⌘+B)
3. Product → Run (⌘+R)

---

### Visual Studio (Windows)

**Open CMake Project:**
1. File → Open → CMake...
2. Select `CMakeLists.txt`
3. VS automatically configures

**Build:**
- Build → Build All (Ctrl+Shift+B)
- Debug → Start Debugging (F5)

**Configuration:**
- Edit `CMakeSettings.json` for custom settings

## Troubleshooting Build Issues

### Issue: CMake Not Found

**Error:**
```
bash: cmake: command not found
```

**Solution:**
```bash
# Ubuntu/Debian
sudo apt install cmake

# macOS
brew install cmake

# Windows
# Download from https://cmake.org/download/
```

---

### Issue: Ninja Not Found

**Error:**
```
CMake Error: CMake was unable to find a build program corresponding to "Ninja"
```

**Solution:**
```bash
# Linux
sudo apt install ninja-build

# macOS
brew install ninja

# Windows (with Chocolatey)
choco install ninja

# Or use different generator:
cmake -G "Unix Makefiles" ..
```

---

### Issue: raylib Not Found

**Error:**
```
Could not find package raylib
```

**Solution:**

CMake should automatically fetch raylib. If it doesn't:

```bash
# Manual installation
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
sudo ninja install
```

---

### Issue: OpenGL Errors on Linux

**Error:**
```
Failed to initialize OpenGL context
```

**Solution:**
```bash
# Install Mesa drivers
sudo apt install mesa-utils libgl1-mesa-dev libglu1-mesa-dev

# Test OpenGL
glxinfo | grep "OpenGL version"

# Update graphics drivers
sudo ubuntu-drivers autoinstall  # Ubuntu
```

---

### Issue: Sound Not Working

**Error:**
```
Failed to initialize audio device
```

**Solution:**
```bash
# Linux: Install ALSA
sudo apt install libasound2-dev

# Test audio
aplay -l

# Check PulseAudio
systemctl --user status pulseaudio
```

---

### Issue: Permission Denied (Linux)

**Error:**
```
Permission denied: ./raylib_game
```

**Solution:**
```bash
# Make executable
chmod +x raylib_game

# Or run with explicit interpreter
./raylib_game
```

---

### Issue: Missing Resources

**Error:**
```
WARNING: Failed to load 'resources/mecha.png'
```

**Solution:**
```bash
# Ensure resources directory exists
ls resources/

# Copy resources to build directory
cp -r ../src/resources .

# Or run from src directory
cd ../src
./raylib_game
```

---

### Issue: Linking Errors (Windows)

**Error:**
```
undefined reference to `WinMain'
```

**Solution:**

Ensure correct subsystem is set:
```cmake
# In CMakeLists.txt
if(WIN32)
    add_executable(raylib_game WIN32 ${SOURCES})
else()
    add_executable(raylib_game ${SOURCES})
endif()
```

## Development Setup

### Recommended Directory Structure

```
/workspace/
├── build/              # Build artifacts (gitignored)
├── build-web/          # Web build (gitignored)
├── docs/               # Documentation
├── src/                # Source code
│   ├── resources/      # Game assets
│   ├── *.c             # Source files
│   └── *.h             # Header files
├── tests/              # Unit tests (optional)
├── CMakeLists.txt      # Build configuration
├── README.md
└── .gitignore
```

### Development Workflow

1. **Make Changes:**
   ```bash
   vim src/tetris.c
   ```

2. **Build:**
   ```bash
   cd build
   ninja
   ```

3. **Test:**
   ```bash
   ./raylib_game
   ```

4. **Debug (with GDB):**
   ```bash
   gdb ./raylib_game
   (gdb) run
   (gdb) backtrace  # On crash
   ```

5. **Iterate:** Repeat steps 1-3

### Hot Reload (Optional)

Use a file watcher for automatic rebuilds:

```bash
# Install watchexec
cargo install watchexec-cli

# Auto-rebuild on changes
watchexec -w src -e c,h "cd build && ninja"
```

### Code Formatting

```bash
# Install clang-format
sudo apt install clang-format

# Format all source files
find src -name "*.c" -o -name "*.h" | xargs clang-format -i
```

### Static Analysis

```bash
# Install cppcheck
sudo apt install cppcheck

# Run analysis
cppcheck --enable=all --suppress=missingIncludeSystem src/
```

## Deployment

### Desktop Distribution

#### Linux (AppImage)

```bash
# Build statically
cmake -DCMAKE_BUILD_TYPE=Release ..
ninja

# Create AppImage (requires appimagetool)
mkdir -p AppDir/usr/bin
mkdir -p AppDir/usr/share/applications
cp raylib_game AppDir/usr/bin/
# ... add .desktop file and icon ...
appimagetool AppDir tetris.AppImage
```

#### macOS (DMG)

```bash
# Create app bundle
cmake -G Xcode ..
xcodebuild -configuration Release

# Create DMG
hdiutil create -volname "Tetris" -srcfolder Release/raylib_game.app \
               -ov -format UDZO tetris.dmg
```

#### Windows (Installer)

Use NSIS or Inno Setup:

```nsi
; tetris.nsi (NSIS script)
Name "Tetris Game"
OutFile "tetris-installer.exe"
InstallDir "$PROGRAMFILES\Tetris"

Section "Install"
    SetOutPath $INSTDIR
    File "raylib_game.exe"
    File "resources\*"
    CreateShortcut "$DESKTOP\Tetris.lnk" "$INSTDIR\raylib_game.exe"
SectionEnd
```

### Web Deployment

1. Build for web (see Web section above)
2. Upload files to hosting:
   - Static host: Netlify, Vercel, GitHub Pages
   - Cloud: AWS S3, Google Cloud Storage

**Example (GitHub Pages):**
```bash
# Build
emcmake cmake ..
ninja

# Deploy
cp raylib_game.* /path/to/gh-pages/
cd /path/to/gh-pages/
git add .
git commit -m "Update game"
git push origin gh-pages
```

### Versioning

Add version to CMakeLists.txt:

```cmake
project(raylib_game VERSION 1.0.0)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
```

`src/config.h.in`:
```c
#define VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define VERSION_MINOR @PROJECT_VERSION_MINOR@
#define VERSION_PATCH @PROJECT_VERSION_PATCH@
```

---

**Last Updated:** 2025-01-01  
**Author:** Utsav Nandi
