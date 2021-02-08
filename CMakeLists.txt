cmake_minimum_required(VERSION 3.10)

project(ComputerGraphics)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(include)

add_executable(lines src/targets/lines.cpp src/model.cpp src/shapes.cpp src/util.cpp src/samples.cpp)
target_link_libraries(lines ${OpenCV_LIBS})

add_executable(triangles src/targets/triangles.cpp src/model.cpp src/shapes.cpp src/util.cpp src/samples.cpp)
target_link_libraries(triangles ${OpenCV_LIBS})