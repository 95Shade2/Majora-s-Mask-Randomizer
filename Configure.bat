@echo off
setlocal EnableExtensions

set "REPO=%~dp0"
set "REPO=%REPO:~0,-1%"

subst M: /d >nul 2>&1
subst M: "%REPO%"
if errorlevel 1 (
    echo Failed to map M: to the repository.
    exit /b 1
)

set "CMAKE=cmake"
where cmake >nul 2>&1
if errorlevel 1 (
    if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
        set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
    )
)

set "CONFIG=Debug"
if /I "%~1"=="Release" set "CONFIG=Release"
if /I "%~1"=="release" set "CONFIG=Release"

echo Configuring...
"%CMAKE%" -S M:\ -B M:\build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 exit /b 1

echo Building %CONFIG%...
"%CMAKE%" --build M:\build --config %CONFIG%
exit /b %errorlevel%
