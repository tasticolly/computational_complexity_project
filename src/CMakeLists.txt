cmake_minimum_required(VERSION 3.20)
project(RuralPostmanProblem)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall -Wextra")

add_executable(RuralPostmanProblem main.cpp)

