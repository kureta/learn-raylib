cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(example)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif ()
endif ()

FetchContent_Declare(
        Torch
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://download.pytorch.org/libtorch/cu121/libtorch-shared-with-deps-2.1.1%2Bcu121.zip
)
FetchContent_MakeAvailable(Torch)
list(APPEND CMAKE_PREFIX_PATH "${torch_SOURCE_DIR}")

find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Our Project
add_executable(${PROJECT_NAME} main.cpp RayApp.cpp RayApp.h raygui.h
        BigGAN.cpp
        BigGAN.h
        Animator.cpp
        Animator.h)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib "${TORCH_LIBRARIES}")

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif ()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()