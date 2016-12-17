@setlocal enableextensions enabledelayedexpansion
@echo off

mkdir c:\projects\pksav\test-env\build

cd c:\projects\pksav\test-env\build
if not !ERRORLEVEL!==0 goto fail

:: MinGW builds can't have sh.exe in the PATH, which is the case by default

if not !ERRORLEVEL!==0 goto fail
if "x%GENERATOR_NAME%"=="xMinGW Makefiles" (
    set PATH="C:\mingw-w64\i686-5.3.0-posix-dwarf-rt_v4-rev0\mingw32\bin;C:\Program Files (x86)\CMake\bin"
)

cmake -G "%GENERATOR_NAME%" -DCMAKE_BUILD_TYPE=Release ..\..

:: Generator-specific build command
if "x%GENERATOR_NAME%"=="xMinGW Makefiles" (
    mingw32-make
) else (
    msbuild /p:configuration=Release ALL_BUILD.vcxproj
)
if not !ERRORLEVEL!==0 goto fail

:: Don't run anything when cross-compiling
if "x%GENERATOR_NAME:ARM=%"=="x%GENERATOR_NAME%" (
    ctest --output-on-failure
    if not !ERRORLEVEL!==0 goto fail
)

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
