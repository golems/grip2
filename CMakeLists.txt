cmake_minimum_required(VERSION 2.8.3)
project(grip2)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------

# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

# Find OpenSceneGraph
find_package(OpenSceneGraph)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})
add_definitions(${OpenSourceGraph_DEFINITIONS})


#-----------------------------------------------
#                SET VARIABLES
#-----------------------------------------------

# Set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)


#-----------------------------------------------
#                SET LIBRARIRES
#-----------------------------------------------

file(GLOB lib_source "src/*.cpp" "include/*.h")
list(SORT lib_source)

# Add source libraries to project library
add_library(${PROJECT_NAME} SHARED ${lib_source})

# Link to required libraries
link_libraries(osg osgViewer osgManipulator osgGA osgDB osgQt ${QT_LIBRARIES})


#-----------------------------------------------
#                BUILD TESTS
#-----------------------------------------------

file(GLOB unit_test_source "test/*.cpp")
LIST(SORT unit_test_source)
add_executable(osgviewerQt "test/osgviewerQt.cpp")

# Build test programs
message(STATUS "\n -- UNIT TEST: ")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
foreach(utest_src_file ${unit_tests_source})
    get_filename_component(test_base ${utest_src_file} NAME_WE)
    message(STATUS "Adding test ${test_base}")
    add_executable(${test_base} ${utest_src_file})
#    target_link_libraries(${test_base} ${PROJECT_NAME} ${OpenSceneGraph_LIBRARIES})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(utest_src_file)

