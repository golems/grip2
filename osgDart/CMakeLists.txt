cmake_minimum_required(VERSION 2.8.3)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find Eigen3
find_package(Eigen REQUIRED)
include_directories(${EIGEN_INCLUDE_DIRS})
add_definitions(${EIGEN_DEFINITIONS})

# Find DART
if(DART_NOT_FOUND)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODLUE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
    find_package(DART REQUIRED)
    include_directories(${DART_INCLUDE_DIRS})
    link_directories(${DART_LIBRARY_DIRS})
endif()

# Include include directory
include_directories(./)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../osgGolems/)


#-----------------------------------------------
#                CREATE LIBRARY
#-----------------------------------------------
# DART OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library(osgDart SHARED ${srcs})
target_link_libraries(osgDart ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})
