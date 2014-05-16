cmake_minimum_required(VERSION 2.8.3)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------

include_directories(./)

# OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library(osgGolems SHARED ${srcs})
target_link_libraries(osgGolems ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})

file(GLOB unit_tests_source "test/*.cpp")
LIST(SORT unit_tests_source)

set(tests_source ${tests_source} ${unit_tests_source} PARENT_SCOPE)
