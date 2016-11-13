@setlocal enableextensions enabledelayedexpansion
@echo off

mkdir c:\projects\pksav\test-env\build

cd c:\projects\pksav\test-env\build
if not !ERRORLEVEL!==0 goto fail

cmake -G "%CMAKE_DASH_G%" -DCMAKE_BUILD_TYPE=Release ..\..
if not !ERRORLEVEL!==0 goto fail

msbuild /p:configuration=Release ALL_BUILD.vcxproj
if not !ERRORLEVEL!==0 goto fail

:: Don't run anything when cross-compiling
if "x%CMAKE_DASH_G:ARM=%"=="x%CMAKE_DASH_G%" (
    ctest --output-on-failure
    if not !ERRORLEVEL!==0 goto fail
)

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
