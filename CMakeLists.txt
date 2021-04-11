cmake_minimum_required(VERSION 3.12)

project(Nova)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----- Nova -----

add_library(
    Nova
    src/Sequencer.cpp
)

target_include_directories(
    Nova
    PRIVATE src
    PUBLIC include
)

# ----- Driver -----

add_executable(
    Driver
    driver/main.cpp
)

target_link_libraries(
    Driver
    PRIVATE Nova
)

# ----- Tests -----

# TODO