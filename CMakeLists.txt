cmake_minimum_required(VERSION 3.10)

project(ComputerGraphics)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(include ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(lines src/targets/lines.cpp src/model.cpp src/shapes.cpp src/util.cpp src/samples.cpp)
target_link_libraries(lines ${OpenCV_LIBS})

add_executable(triangles src/targets/triangles.cpp src/model.cpp src/shapes.cpp src/util.cpp src/samples.cpp)
target_link_libraries(triangles ${OpenCV_LIBS})

add_executable(mandelbrot src/targets/mandelbrot.cpp)
target_link_libraries(mandelbrot ${OpenCV_LIBS})

add_executable(rotation src/targets/rotation.cpp src/samples.cpp src/shapes.cpp src/util.cpp src/model.cpp)
target_link_libraries(rotation ${OpenCV_LIBS})

add_executable(lighting src/targets/lighting.cpp)
target_link_libraries(lighting ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

add_executable(lighting_model src/targets/lighting_model.cpp src/model.cpp src/util.cpp)
target_link_libraries(lighting_model ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})

add_executable(opengl_objects src/targets/opengl_objects.cpp src/model.cpp src/util.cpp)
target_link_libraries(opengl_objects ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})

add_executable(opengl_fog src/targets/opengl_fog.cpp src/model.cpp src/util.cpp)
target_link_libraries(opengl_fog ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})