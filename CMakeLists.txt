cmake_minimum_required(VERSION 2.8.3)
project(grip2)

#-----------------------------------------------
#                COMPILE SETTINGS
#-----------------------------------------------
set(CMAKE_BUILD_TYPE "Debug")
set(BUILD_FLAGS "-std=c++0x -Wreturn-type -Wno-enum-compare -Wall -03")


#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

# Find OpenSceneGraph
find_package(OpenSceneGraph 2.8 REQUIRED
    COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})
add_definitions(${OpenSourceGraph_DEFINITIONS})
set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)


#-----------------------------------------------
#                SET VARIABLES
#-----------------------------------------------
# Set include directories
set(QT_WIDGETS_DIR ${PROJECT_SOURCE_DIR}/qtWidgets)
set(OSG_NODES_DIR ${PROJECT_SOURCE_DIR}/osgNodes)

include_directories(include ${QT_WIDGETS_DIR} ${OSG_NODES_DIR})
#link_directories(${QT_WIDGETS_DIR} ${OSG_NODES_DIR})


#-----------------------------------------------
#               SET LIBRARIRES
#-----------------------------------------------
# OpenSceneGraph Nodes Library
file(GLOB osg_lib_source ${OSG_NODES_DIR}/*.cpp ${OSG_NODES_DIR}/*.h)

# Add source libraries to project library
add_library(${PROJECT_NAME} SHARED ${osg_lib_source})


#-----------------------------------------------
#                BUILD GRIP
#-----------------------------------------------
QT4_WRAP_CPP(GUI_MOC_HEADERS ${QT_WIDGETS_DIR}/mainwindow.h)
file(GLOB GUI_SRC ${QT_WIDGETS_DIR}/*.h ${QT_WIDGETS_DIR}/*.cpp)
add_executable(grip ${PROJECT_SOURCE_DIR}/grip.cpp ${GUI_SRC} ${GUI_MOC_HEADERS})
target_link_libraries(grip ${QT_LIBRARIES} ${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})


#-----------------------------------------------
#                BUILD TESTS
#-----------------------------------------------
# Get test files
file(GLOB unit_tests_source "test/*.cpp")
LIST(SORT unit_tests_source)

# Build test programs
message(STATUS "\n-- UNIT TEST: ")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
foreach(utest_src_file ${unit_tests_source})
    get_filename_component(test_base ${utest_src_file} NAME_WE)
    message(STATUS "Adding test ${test_base}")
    add_executable(${test_base} ${utest_src_file})
    target_link_libraries(${test_base} ${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(utest_src_file)
