cmake_minimum_required(VERSION 2.8.3)
project(grip2)

# Define some colors for fun output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColorReset "${Esc}[m")
  set(Red         "${Esc}[1;31m")
  set(Yellow      "${Esc}[1;33m")
endif()

if(APPLE)
    set(X11_THREADS_LIB "X11")

    # set X11 include path for Mac OS X 
    include_directories(/opt/X11/include)
    link_directories(/opt/X11/lib)
else(APPLE)
    # linux uses rt for XInitThreads()
    set(X11_THREADS_LIB "rt")
endif()

#-----------------------------------------------
#                COMPILE SETTINGS
#-----------------------------------------------
set(CMAKE_BUILD_TYPE "Debug")
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL QtXml REQUIRED) # Qt3Support
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

set(OSG_LIBRARY_INSTALL "TRUE")
if(${OSG_LIBRARY_INSTALL})
    # Find OpenSceneGraph (as installed library 3.0.1, for ubuntu 14.04)
    # NOTE: The osg deb for 14.04 is v3.2.0, which is broken (renders weird) 
    #       * to get OSG 3.0.1 to build you may need to disable ffmpeg:
    #       * run "ccmake ..", and delete entry "FFMPEG_LIBAVUTIL_INCLUDE_DIRS" 
    find_library(OpenSceneGraph REQUIRED
        COMPONENTS osg osgViewer osgManipulator osgGA osgDB osgQt)
    set(OSG_LIBRARY_PATH "/usr/local/lib") # may have to be customized
    if(APPLE)
        set(LIB_SUFFIX "dylib")
    else(APPLE)
        set(LIB_SUFFIX "so")
    endif()
    set(OPENSCENEGRAPH_LIBRARIES ${OSG_LIBRARY_PATH}/libosg.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosgViewer.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosgManipulator.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosgGA.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosgDB.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libOpenThreads.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosgQt.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosg.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libosg.${LIB_SUFFIX}
        ${OSG_LIBRARY_PATH}/libOpenThreads.${LIB_SUFFIX}) 
else(${OSG_LIBRARY_INSTALL})
    # Find OpenSceneGraph (cmake package)
    find_package(OpenSceneGraph 3.0 REQUIRED
        COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
    find_package(OpenSceneGraph 3.0 QUIET COMPONENTS osgQt)
    include_directories(${OpenSceneGraph_INCLUDE_DIRS})
    add_definitions(${OpenSourceGraph_DEFINITIONS})
    set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)
endif()

# Find Eigen3
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Eigen REQUIRED)
include_directories(${Eigen_INCLUDE_DIR})

# Find DART
find_package(DART REQUIRED)
include_directories(${DART_INCLUDE_DIRS})
link_directories(${DART_LIBRARY_DIRS})

#-----------------------------------------------
#      INCLUDES & SUBDIRECTORY LIBRARIRES
#-----------------------------------------------
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/qtWidgets)

# Add QT Widgets library
add_subdirectory(qtWidgets)
include_directories(qtWidgets)

# Add OpenSceneGraph Nodes library
add_subdirectory(osgGolems)
include_directories(osgGolems)

# Add DART OSG library
add_subdirectory(osgDart)
include_directories(osgDart)

# Set project libraries variable
set(project_libs qtWidgets osgGolems osgDart)

# Add Main Window library
QT4_WRAP_CPP(GUI_MOC_HEADERS ${PROJECT_SOURCE_DIR}/include/MainWindow.h ${PROJECT_SOURCE_DIR}/include/GripSimulation.h)
file(GLOB GUI_SRC include/*.h src/*.cpp)
LIST(REMOVE_ITEM GUI_SRC "${CMAKE_CURRENT_LIST_DIR}/include/TestMainWindow.h"
                           "${CMAKE_CURRENT_LIST_DIR}/src/TestMainWindow.cpp")
add_library(mainWindow SHARED ${GUI_MOC_HEADERS} ${GUI_SRC})
target_link_libraries(mainWindow ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${X11_THREADS_LIB})

# Set project libraries variable
set(project_libs ${project_libs} mainWindow)

#-----------------------------------------------
#                BUILD GRIP
#-----------------------------------------------
# QT4_WRAP_CPP(TEST_MOC_HEADERS ${PROJECT_SOURCE_DIR}/include/TestMainWindow.h)
# FILE(GLOB TEST_SRC src/TestMainWindow.cpp)
# add_library(grip-test SHARED ${TEST_MOC_HEADERS} ${TEST_SRC})
# target_link_libraries(grip-test ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

add_executable(grip ${PROJECT_SOURCE_DIR}/src/grip.cpp)
target_link_libraries(grip ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

FILE(GLOB grip-srcs osgGolems/*.h osgGolems/src/*.cpp osgDart/*.h osgDart/src/*.cpp qtWidgets/*.h qtWidgets/src/*.cpp)
add_library(grip-core SHARED ${grip-srcs})
target_link_libraries(grip-core ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

add_library(grip-interface SHARED src/GripInterface.cpp include/GripInterface.h)
target_link_libraries(grip-interface ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})


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
# Install grip
INSTALL(PROGRAMS build/grip
        DESTINATION /usr/local/bin/)

# Install library of grips libraries
INSTALL(TARGETS grip-core
        DESTINATION /usr/local/lib/)

# Install header files by namespace and directory
FILE(GLOB osgDartHeaders "osgDart/*.h")
INSTALL(FILES ${osgDartHeaders}
        DESTINATION /usr/local/include/grip/osgDart)

FILE(GLOB osgGolemsHeaders "osgGolems/*.h")
INSTALL(FILES ${osgGolemsHeaders}
        DESTINATION /usr/local/include/grip/osgGolems)

FILE(GLOB qtWidgetsHeaders "qtWidgets/*.h")
INSTALL(FILES ${qtWidgetsHeaders}
        DESTINATION /usr/local/include/grip/qtWidgets)

FILE(GLOB mainHeaders "include/*.h")
INSTALL(FILES ${mainHeaders}
        DESTINATION /usr/local/include/grip/include)

# Install cmake find file for grip
INSTALL(FILES cmake/FindGRIP.cmake
        DESTINATION ${CMAKE_ROOT}/Modules)

# Install desktop file to call grip from desktop
INSTALL(FILES "doc/grip.desktop"
        DESTINATION /usr/share/applications)

INSTALL(FILES "src/icons/grip-icon.png"
        DESTINATION /usr/share/icons/)
