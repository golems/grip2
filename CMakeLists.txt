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
#            INCLUDES & LIBRARIES
#-----------------------------------------------
# Include top-level include directory
include_directories(include)

# Add QT Widgets library
add_subdirectory(qtWidgets)
include_directories(qtWidgets/include)

# Add OpenSceneGraph Nodes library
add_subdirectory(osgNodes)
include_directories(osgNodes/include)

# Add Main Window library
QT4_WRAP_CPP(GUI_MOC_HEADERS ${PROJECT_SOURCE_DIR}/include/mainwindow.h)
file(GLOB GUI_SRC include/*.h src/*.cpp)
add_library(mainwindow ${GUI_MOC_HEADERS} ${GUI_SRC})
target_link_libraries(mainwindow qtWidgets osgNodes)

# Set project libraries variable
set(project_libs qtWidgets osgNodes mainwindow)


#-----------------------------------------------
#                BUILD GRIP
#-----------------------------------------------
add_executable(grip ${PROJECT_SOURCE_DIR}/src/grip.cpp)
target_link_libraries(grip ${project_libs} ${QT_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})


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
    target_link_libraries(${test_base} ${project_libs} ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(utest_src_file)
