cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

file(GLOB INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIRS})

add_executable(task1 main.cpp
               src/ArcGraph.cpp
               src/ListGraph.cpp
               src/MatrixGraph.cpp
               src/SetGraph.cpp
)
