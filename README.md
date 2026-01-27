# Paper Marbling

An interactive visualization of the paper marbling technique using fluid dynamics algorithms. This project simulates ink drops on water, creating beautiful marbling patterns in real-time.

## About This Project

Paper Marbling is a real-world art technique where colored inks are dropped onto water and manipulated using combs (tines) to create intricate patterns. This project digitally recreates that artistic process using mathematical models to simulate how ink droplets interact with each other.

The implementation uses two key mathematical techniques:

- **Jaffers Formula**: Calculates how vertices on one ink drop deform when influenced by another drop's position and radius
- **Generalized Tine Algorithm**: Models the effect of combs on the ink, working in any direction (not just vertical or horizontal)

## Credits

This project was inspired by **[The Coding Train](https://www.thecodingrain.com/)** and their excellent "Coding Challenges" series. Special thanks to their educational approach to creative coding and physics simulations. The mathematical models used in this implementation are based on techniques featured in their content and papers on fluid dynamics visualization.

## Technologies Used

- **C++20**: Modern C++ for performance and safety
- **Raylib**: Graphics and window management
- **Dear ImGui**: Interactive control panel UI
- **CMake**: Cross-platform build system

## Features

- Interactive ink drop placement via mouse or automatic spawning
- Real-time marble effect calculations as drops interact
- Tine (comb) tool for manipulating ink patterns
- Customizable color palette with color picker
- Adjustable background color
- FPS and drop count monitoring
- Cross-platform support (Windows, macOS, Linux)

---

## Prerequisites

Before building, ensure you have the following installed:

* **CMake 3.21+**
* **A C++20 compiler** (MSVC, MinGW-w64, Clang, or GCC)
* **Git** (required for FetchContent to download dependencies)
* **OpenGL support**

### Platform-Specific Notes

**Windows (MSVC)**:
- Visual Studio 2019 or newer with C++ development tools
- OpenGL support is typically included

**Windows (MinGW)**:
- MinGW-w64 with g++ C++20 support

**macOS**:
- Xcode Command Line Tools: `xcode-select --install`
- Clang compiler with C++20 support

**Linux**:
- Build tools: `sudo apt-get install build-essential cmake git`
- OpenGL libraries: `sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev`
- GLFW dependencies: `sudo apt-get install libxrandr-dev libxinerama-dev libxi-dev libxext-dev libxcursor-dev`

**Optional** (for faster rebuilds): Install **ccache**

---

## Downloading Pre-built Releases

Pre-built binaries for Windows, macOS, and Linux are available from the [Releases](https://github.com/CableCry/PaperMarbling/releases) page.

1. Download the appropriate archive for your platform:
   - `PaperMarbling-Windows.zip` - Windows (x64)
   - `PaperMarbling-Linux.tar.gz` - Linux (x64)
   - `PaperMarbling-macOS.tar.gz` - macOS (x64 and ARM64)

2. Extract the archive

3. Run the executable:
   - **Windows**: Double-click `rayproj.exe`
   - **Linux/macOS**: Open terminal in the extracted folder and run `./rayproj`

> **Note**: On macOS, you may need to allow the application in System Preferences > Security & Privacy if you get a security warning.
> On Linux, you may need to make the executable executable: `chmod +x rayproj`

---

## Building the Project

### Quick Start (Using Build Scripts)

The easiest way to build is using the provided build scripts:

**Windows:**
```cmd
build.bat Release
```

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh Release
```

Then run the application from the `bin/` directory:
```bash
cd bin && ./rayproj     # Linux/macOS
cd bin && rayproj.exe   # Windows
```

### Manual Build Process

#### Step 1: Clone the Repository

```bash
git clone https://github.com/CableCry/PaperMarbling.git
cd PaperMarbling
```

#### Step 2: Configure with CMake

The project uses CMake presets for easy configuration.

**Debug configuration:**
```bash
cmake --preset debug
```

**Release configuration:**
```bash
cmake --preset release
```

> **Note**: On first run, CMake will automatically fetch and build **Raylib**, **Dear ImGui**, and **rlImGui** from GitHub. This may take a few minutes depending on your internet speed and system performance.

#### Step 3: Build the Project

**On Windows:**
```bash
cmake --build build/release --config Release
```

**On Linux & macOS:**
```bash
cmake --build build/release --config Release -j$(nproc)
```

#### Step 4: Run the Executable

After building, the executable will be located in the `bin/` directory.

**Windows:**
```bash
.\bin\rayproj.exe
```

**macOS & Linux:**
```bash
./bin/rayproj
```

---

## Usage

Once the application is running:

- **Left Click**: Place an ink drop at the mouse position
- **Click & Hold**: Continuously spawn drops at the mouse location
- **Scroll Wheel**: Adjust the tine Z-value (effect strength)
- **ImGui Controls**: 
  - Adjust the background clear color
  - Create and manage custom ink colors
  - View real-time FPS and drop count

---

## Project Structure

```
PaperMarbling/
├── CMakeLists.txt           # Main build configuration
├── CMakePresets.json        # Build presets (debug/release)
├── src/
│   ├── main.cpp             # Application entry point and UI
│   ├── ink.cpp              # Drop physics and marbling calculations
│   ├── ink.hpp              # Drop class definition
│   └── shaders/             # GLSL shader files
├── cmake/
│   ├── Dependencies.cmake    # Fetches Raylib, ImGui, rlImGui
│   └── Warnings.cmake        # Compiler warning configuration
├── assets/
│   ├── fonts/               # Font files for UI
│   └── textures/            # Texture assets
└── bin/                      # Output directory for executable
```

---

## Building From Scratch (Full Clean Build)

If you encounter build issues or want a completely fresh build:

**Windows:**
```cmd
rmdir /s /q build
cmake --preset release
cmake --build build/release --config Release
```

**Linux/macOS:**
```bash
rm -rf build/
cmake --preset release
cmake --build build/release --config Release -j$(nproc)
```

---

## Creating a Release

To create a packaged release:

```bash
# After building with Release preset
cd build/release
cpack
```

This will create platform-appropriate packages:
- **Windows**: ZIP archive
- **macOS**: TGZ and DMG (DragNDrop) archives
- **Linux**: TGZ archive

---

## Performance Notes

- The project targets 60 FPS with vsync enabled
- On systems with many active drops (100+), consider using the Release build for better performance
- Tine Z value affects visual quality and performance

---

## License

This project uses open-source libraries. See their respective licenses:
- **Raylib**: zlib License
- **Dear ImGui**: MIT License
