cmake_minimum_required(VERSION 3.15)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
project(Snow C)

set(CMAKE_C_STANDARD 99)

add_executable(Snow main.c)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} /usr/local/include)
target_link_libraries(Snow ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(Snow /usr/local/lib/libSOIL.a)
