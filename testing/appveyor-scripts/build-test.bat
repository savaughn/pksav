mkdir c:\projects\pksav\build
cd c:\projects\pksav\build
cmake -G %CMAKE_DASH_G% ..
msbuild ALL_BUILD.vcxproj
ctest --output-on-failure
