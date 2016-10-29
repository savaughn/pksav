mkdir c:\projects\pksav\build
cd c:\projects\pksav\build
cmake -G "Visual Studio 14 2015 Win64" ..
msbuild ALL_BUILD.vcxproj
ctest --output-on-failure
