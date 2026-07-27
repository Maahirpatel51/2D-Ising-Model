
# Set minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and language
project(Ising2D LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executable target and source files
add_executable(Ising2D
    src/main.cpp
    src/IsingModel.cpp
)