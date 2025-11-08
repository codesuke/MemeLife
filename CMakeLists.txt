cmake_minimum_required(VERSION 3.10)
project(MemeLife)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Terminal.cpp
    src/AssetLoader.cpp
    src/Renderer.cpp
    src/Map.cpp
    src/Quiz.cpp
    src/Battle.cpp
    src/Save.cpp
)

# Create executable
add_executable(MemeLife ${SOURCES})

# Platform-specific settings
if(WIN32)
    # Windows doesn't need additional libraries for conio.h
else()
    # Unix-like systems might need additional libraries
endif()
