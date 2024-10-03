cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

# set(PROJECT_NAME)
project(Genetic_ASCII VERSION 1.0)

set(SOURCES
src/main.cpp
src/agent.h
src/agent.cpp
)

add_executable(Genetic_ASCII ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME})