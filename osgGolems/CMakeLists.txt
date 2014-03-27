cmake_minimum_required(VERSION 2.8.3)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------

include_directories(include)

# OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
add_library(osgGolems SHARED ${srcs})
target_link_libraries(osgGolems ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
