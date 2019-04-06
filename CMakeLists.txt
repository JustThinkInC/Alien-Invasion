cmake_minimum_required(VERSION 2.8)

project(Assignment1)

add_executable(Cannon.out model/Cannon/Cannon.cpp)
add_executable(Castle.out model/Castle/Castle.cpp)
add_executable(Robots.out model/Robots/Robots.cpp model/Shapes/Shapes.cpp)
add_executable(Main.out controller/main.cpp model/Shapes/Shapes.cpp model/Castle/Castle.cpp model/robots/Robots.cpp
        model/Cannon/Cannon.cpp model/SpaceShip/Spaceship.cpp)
#add_executable(Humanoid.out Humanoid.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Cannon.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries( Castle.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries( Robots.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries( Main.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
#target_link_libraries( Humanoid.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
