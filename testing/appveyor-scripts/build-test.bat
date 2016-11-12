@setlocal enableextensions enabledelayedexpansion

mkdir c:\projects\pksav\test-env\build
cd c:\projects\pksav\test-env\build
cmake -G "%CMAKE_DASH_G%" ..\..
msbuild ALL_BUILD.vcxproj

if "x%CMAKE_DASH_G:ARM=%"=="x%CMAKE_DASH_G%" (
    ctest --output-on-failure
)

endlocal
