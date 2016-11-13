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

    set SAVEDIR=c:\projects\pksav\testing\pksav-test-saves
    set PATH="%PATH%";c:\projects\pksav\test-env\build\lib\Release;c:\projects\pksav\test-env\build\apps\Release

    pksav-gen1-save-dump --all --input="!SAVEDIR!\red_blue\pokemon_red.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gen1-save-dump --all --input="!SAVEDIR!\yellow\pokemon_yellow.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gen2-save-dump "!SAVEDIR!\gold_silver\pokemon_gold.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gen2-save-dump "!SAVEDIR!\crystal\pokemon_crystal.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gba-save-dump --all --input="!SAVEDIR!\ruby_sapphire\pokemon_ruby.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gba-save-dump --all --input="!SAVEDIR!\emerald\pokemon_emerald.sav"
    if not !ERRORLEVEL!==0 goto fail

    pksav-gba-save-dump --all --input="!SAVEDIR!\firered_leafgreen\pokemon_firered.sav"
    if not !ERRORLEVEL!==0 goto fail
)

goto pass

endlocal

:fail
exit /b 1

:pass
exit /b 0
