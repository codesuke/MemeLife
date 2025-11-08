cmake_minimum_required(VERSION 3.10)

# Set the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(MemeLifeSDL2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Find SDL2_ttf - manually set path since it's not in vcpkg
find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL_ttf.h
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include
)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib
)

if(SDL2_TTF_INCLUDE_DIR AND SDL2_TTF_LIBRARY)
    message(STATUS "Found SDL2_ttf: ${SDL2_TTF_LIBRARY}")
    add_library(SDL2::SDL2_ttf UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2_ttf PROPERTIES
        IMPORTED_LOCATION "${SDL2_TTF_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_TTF_INCLUDE_DIR}"
    )
else()
    message(FATAL_ERROR "SDL2_ttf not found. Headers or libraries are missing.")
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Map.cpp
    src/Map.h
    src/Player.cpp
    src/Player.h
    src/AudioManager.cpp
    src/AudioManager.h
    src/TimeManager.cpp
    src/TimeManager.h
    src/Font.cpp
    src/Font.h
    src/DialogueBox.cpp
    src/DialogueBox.h
    src/UIManager.cpp
    src/UIManager.h
    src/HUDManager.cpp
    src/HUDManager.h
    src/EventManager.cpp
    src/EventManager.h
    src/GameManager.cpp
    src/GameManager.h
    src/Buff.cpp
    src/Buff.h
    src/CharacterCreationScene.cpp
    src/CharacterCreationScene.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2::SDL2_ttf
)