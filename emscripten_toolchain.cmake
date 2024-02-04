set(CMAKE_C_COMPILER "emcc")
set(CMAKE_CXX_COMPILER "em++")
set(CMAKE_AR "emar")
set(CMAKE_RANLIB "emranlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s WASM=1 -I/opt/homebrew/Cellar/emscripten/3.1.50/libexec/system/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")

# Add other Emscripten-specific flags as needed

set(CMAKE_EXECUTABLE_SUFFIX ".html")
