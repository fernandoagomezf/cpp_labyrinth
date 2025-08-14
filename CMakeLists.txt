cmake_minimum_required(VERSION 3.10)

# MUST come before project()
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(LabyrinthGame)

set(CMAKE_CXX_STANDARD 17)

# Explicit SDL2 config paths (update these if different)
set(SDL2_DIR "C:/vcpkg/installed/x64-windows/share/sdl2")
set(SDL2_IMAGE_DIR "C:/vcpkg/installed/x64-windows/share/sdl2-image")
set(SDL2_TTF_DIR "C:/vcpkg/installed/x64-windows/share/sdl2-ttf")

# Override CMake's default search paths
list(APPEND CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

# Find packages (REQUIRED makes missing packages fail early)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Convert CMake variables to full paths
set(SDL2_LIB_PATH "${SDL2_LIBRARIES}")
set(SDL2_IMAGE_LIB_PATH "C:/vcpkg/installed/x64-windows/lib/SDL2_image.lib")
set(SDL2_TTF_LIB_PATH "C:/vcpkg/installed/x64-windows/lib/SDL2_ttf.lib")

# Verify paths exist
if(NOT EXISTS ${SDL2_IMAGE_LIB_PATH})
    message(FATAL_ERROR "SDL2_image.lib not found at: ${SDL2_IMAGE_LIB_PATH}")
endif()

# Debug output (verify paths)
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

add_executable(LabyrinthGame
    src/main.cpp 
    src/gameapp.cpp
)

# Add this line to specify include directories
target_include_directories(LabyrinthGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # For common.h
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Keep only ONE target_link_libraries section
target_link_libraries(LabyrinthGame PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)