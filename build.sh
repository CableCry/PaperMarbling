#!/bin/bash
# Build script for Linux/macOS

set -e

BUILD_TYPE=${1:-Release}

echo "Building PaperMarbling for $(uname -s) in ${BUILD_TYPE} mode..."

# Check for dependencies
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake not found. Please install CMake 3.21 or higher."
    exit 1
fi

if ! command -v ninja &> /dev/null; then
    echo "Warning: Ninja not found. CMake will use default generator."
    GENERATOR=""
else
    GENERATOR="-G Ninja"
fi

# Configure
echo "Configuring..."
if [ "$BUILD_TYPE" = "Release" ]; then
    cmake --preset release $GENERATOR
else
    cmake --preset debug $GENERATOR
fi

# Build
echo "Building..."
if [ "$BUILD_TYPE" = "Release" ]; then
    cmake --build build/release --config Release
else
    cmake --build build/debug --config Debug
fi

echo ""
echo "Build complete! Executable location:"
echo "  $(pwd)/bin/rayproj"
echo ""
echo "To run the application:"
echo "  cd bin && ./rayproj"
