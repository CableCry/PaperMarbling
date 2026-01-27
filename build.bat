@echo off
REM Build script for Windows

setlocal

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

echo Building PaperMarbling for Windows in %BUILD_TYPE% mode...

REM Check for CMake
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake not found. Please install CMake 3.21 or higher.
    exit /b 1
)

REM Check for Ninja
where ninja >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Warning: Ninja not found. CMake will use default generator.
    set GENERATOR=
) else (
    set GENERATOR=-G Ninja
)

REM Configure
echo Configuring...
if "%BUILD_TYPE%"=="Release" (
    cmake --preset release %GENERATOR%
) else (
    cmake --preset debug %GENERATOR%
)

if %ERRORLEVEL% NEQ 0 (
    echo Configuration failed!
    exit /b 1
)

REM Build
echo Building...
if "%BUILD_TYPE%"=="Release" (
    cmake --build build/release --config Release
) else (
    cmake --build build/debug --config Debug
)

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build complete! Executable location:
echo   %CD%\bin\rayproj.exe
echo.
echo To run the application:
echo   cd bin ^&^& rayproj.exe

endlocal
