@setlocal enableextensions enabledelayedexpansion

mkdir c:\projects\pksav\test-env\build

cd c:\projects\pksav\test-env\build
if not %ERRORLEVEL%==0 goto fail

cmake -G "%CMAKE_DASH_G%" -DCMAKE_BUILD_TYPE=Debug ..\..
if not %ERRORLEVEL%==0 goto fail

msbuild ALL_BUILD.vcxproj /p:configuration=Debug
if not %ERRORLEVEL%==0 goto fail

:: Don't run anything when cross-compiling
if "x%CMAKE_DASH_G:ARM=%"=="x%CMAKE_DASH_G%" (
    ctest --output-on-failure
    if not %ERRORLEVEL%==0 goto fail

    set SAVEDIR=c:\projects\pksav\testing\pksav-test-saves
    set PATH=c:\projects\pksav\test-env\build\lib\Debug;%PATH%
    set PATH=c:\projects\pksav\test-env\build\apps\Debug;%PATH%

    pksav-gen1-save-dump --all --input=%SAVEDIR%\red_blue\pokemon_red.sav
    pksav-gen1-save-dump --all --input=%SAVEDIR%\yellow\pokemon_yellow.sav
    pksav-gen2-save-dump %SAVEDIR%\gold_silver\pokemon_gold.sav
    pksav-gen2-save-dump %SAVEDIR%\crystal\pokemon_crystal.sav
)

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
