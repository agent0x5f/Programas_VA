cmake_minimum_required(VERSION 3.10)
project(histograma)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(histograma
        main.cpp
        io.cpp
        algoritmo.cpp
)

target_link_libraries(histograma ${OpenCV_LIBS})