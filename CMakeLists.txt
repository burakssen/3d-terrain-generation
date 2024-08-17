cmake_minimum_required(VERSION 3.30)

project(3dTerrainGeneration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor/raylib)


file(GLOB_RECURSE SOURCES src/*.cpp)

# Check if we're building for WebAssembly (Emscripten)
if(EMSCRIPTEN)
    add_compile_definitions(PLATFORM_WEB)
    set(BUILD_PLATFORM "PLATFORM=Web")
endif()

add_executable(3dTerrainGeneration ${SOURCES})

target_include_directories(3dTerrainGeneration PRIVATE include vendor/raylib/src)

target_link_libraries(3dTerrainGeneration raylib)

if(EMSCRIPTEN)
    set_target_properties(3dTerrainGeneration PROPERTIES 
        LINK_FLAGS "-s USE_GLFW=3 -s TOTAL_MEMORY=67108864 --shell-file ${CMAKE_SOURCE_DIR}/index.html"
    )

    file(COPY index.html DESTINATION ${CMAKE_BINARY_DIR})
endif()
