cmake_minimum_required(VERSION 2.8.3)
project(grip2)

#-----------------------------------------------
#                COMPILE SETTINGS
#-----------------------------------------------
set(CMAKE_BUILD_TYPE "Release")
set(BUILD_FLAGS "-std=gnu++0x -Wreturn-type -Wno-enum-compare -Wall -O3")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Define preprocessor macro for Debug printing in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "\nBuilding in \"Debug\" mode.\n-- CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "\nBuilding in \"Release\" mode.\n-- CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
else()
endif()

#-----------------------------------------------
#               UNINSTALL TARGET
#-----------------------------------------------
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui Qt3Support QtXml REQUIRED)
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

# Find OpenSceneGraph
find_package(OpenSceneGraph 2.8 REQUIRED
    COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})
add_definitions(${OpenSourceGraph_DEFINITIONS})
set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)

# Find Eigen3
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Eigen REQUIRED)
include_directories(${EIGEN_INCLUDE_DIRS})
add_definitions(${EIGEN_DEFINITIONS})

# Find DART
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODLUE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
find_package(DART REQUIRED)
include_directories(${DART_INCLUDE_DIRS})
link_directories(${DART_LIBRARY_DIRS})


#-----------------------------------------------
#      INCLUDES & SUBDIRECTORY LIBRARIRES
#-----------------------------------------------
# Include top-level include directory
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/qtWidgets)

# Add QT Widgets library
add_subdirectory(qtWidgets)
include_directories(qtWidgets/include)

# Add OpenSceneGraph Nodes library
add_subdirectory(osgGolems)
include_directories(osgGolems/include)

# Add DART OSG library
add_subdirectory(osgDart)
include_directories(osgDart/include)

# Set project libraries variable
set(project_libs qtWidgets osgGolems osgDart)

# Add Main Window library
QT4_WRAP_CPP(GUI_MOC_HEADERS ${PROJECT_SOURCE_DIR}/include/MainWindow.h ${PROJECT_SOURCE_DIR}/include/GripSimulation.h)
file(GLOB GUI_SRC include/*.h src/*.cpp)
add_library(mainWindow ${GUI_MOC_HEADERS} ${GUI_SRC})
target_link_libraries(mainWindow ${project_libs} ${DART_LIBRARIES})

# Set project libraries variable
set(project_libs ${project_libs} mainWindow)


#-----------------------------------------------
#                BUILD GRIP
#-----------------------------------------------
add_executable(grip ${PROJECT_SOURCE_DIR}/src/grip.cpp)
target_link_libraries(grip ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

add_library(griplib SHARED)
target_link_libraries(griplib ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

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
    target_link_libraries(${test_base} ${project_libs} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(utest_src_file)


#-----------------------------------------------
#                 INSTALLATION
#-----------------------------------------------
INSTALL(PROGRAMS build/grip
        DESTINATION /usr/local/bin/)

INSTALL(TARGETS griplib
        DESTINATION /usr/local/lib/)

INSTALL(FILES "qtWidgets/include/GripTab.h"
        DESTINATION /usr/local/include)

INSTALL(FILES "doc/grip.desktop"
        DESTINATION /usr/share/applications)
