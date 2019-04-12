cmake_minimum_required(VERSION 2.8)

project(Assignment1)

add_executable(Main.out controller/main.cpp model/Shapes/Shapes.cpp model/Castle/Castle.cpp model/Robots/Robots.cpp
        model/Cannon/Cannon.cpp model/SpaceShip/Spaceship.cpp model/Skybox/Skybox.cpp model/Wineglass/Wineglass.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Main.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
