@echo off
REM Famantic OS Build Script for Windows (WSL)
REM This script uses WSL to build the OS

echo ========================================
echo    FAMANTIC OS - Windows Build System
echo ========================================
echo.

REM Check if WSL is installed
wsl --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: WSL is not installed!
    echo Please install WSL first: wsl --install
    exit /b 1
)

echo [INFO] WSL detected!
echo [INFO] Building Famantic OS using WSL...
echo.

REM Convert Windows path to WSL path
set "CURRENT_DIR=%CD%"
set "WSL_PATH=/mnt/c/%CURRENT_DIR:C:\=%"
set "WSL_PATH=%WSL_PATH:\=/%"

echo [INFO] Working directory: %WSL_PATH%
echo.

REM Run build script in WSL
wsl bash -c "cd '%WSL_PATH%' && chmod +x build.sh && ./build.sh"

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo ISO file created: Famantic-os.iso
    echo.
    echo You can now use this ISO in VirtualBox!
    echo.
) else (
    echo.
    echo ========================================
    echo    BUILD FAILED!
    echo ========================================
    echo.
    echo Please check the error messages above.
    echo.
)

pause
