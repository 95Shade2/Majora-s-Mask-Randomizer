@echo off

setlocal EnableExtensions



REM MSBuild cannot compile C# when paths contain an apostrophe.

REM Map the repo to M: before configuring/opening the solution.

set "REPO=%~dp0"

set "REPO=%REPO:~0,-1%"



subst M: /d >nul 2>&1

subst M: "%REPO%"

if errorlevel 1 (

    echo Failed to map M: to the repository.

    echo Unmap any existing M: drive, then run this script again.

    exit /b 1

)



set "CMAKE=cmake"

where cmake >nul 2>&1

if errorlevel 1 (

    if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (

        set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (

        set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (

        set "CMAKE=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

    ) else (

        echo CMake was not found. Install Visual Studio 2022 with C++ and .NET desktop workloads.

        exit /b 1

    )

)



echo Configuring CMake...

"%CMAKE%" -S M:\ -B M:\build -G "Visual Studio 17 2022" -A x64

if errorlevel 1 (

    echo CMake configure failed.

    exit /b 1

)



echo Opening M:\build\mm_rando.sln ...

start "" "M:\build\mm_rando.sln"



endlocal

