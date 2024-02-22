cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(typist)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf)
target_sources(${PROJECT_NAME} PRIVATE 
 src/main.cpp
 src/textinput.cpp
 src/game.cpp
 src/random.cpp
 src/particles.cpp
 src/ui.cpp
 src/selection.cpp
)
