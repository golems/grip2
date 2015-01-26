cmake_minimum_required(VERSION 2.8.3)
project(grip)

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
set(CMAKE_BUILD_TYPE "Release")
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -Wall")

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

# Add custom-made Find cmakes
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL QtXml REQUIRED) # Qt3Support
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

# try to find the OpenSceneGraph cmake package 
find_package( OpenSceneGraph REQUIRED COMPONENTS osg osgViewer osgManipulator osgGA osgDB osgQt )

# Find Eigen3
find_package(Eigen REQUIRED)
include_directories(${Eigen_INCLUDE_DIR})

# Find DART
find_package(DART REQUIRED)
include_directories(${DART_INCLUDE_DIRS})
link_directories(${DART_LIBRARY_DIRS})

#-----------------------------------------------
#      INCLUDES & SUBDIRECTORY LIBRARIRES
#-----------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories(${CMAKE_BINARY_DIR}/qtWidgets)

# Add DART OSG library
add_subdirectory(osgDart)
include_directories(osgDart)

# Add OpenSceneGraph Nodes library
add_subdirectory(osgGolems)
include_directories(osgGolems)

# Add QT Widgets library
add_subdirectory(qtWidgets)
include_directories(qtWidgets)

# Set project libraries variable
set(project_libs qtWidgets osgGolems osgDart)

# Add Main Window library
QT4_WRAP_CPP(GUI_MOC_HEADERS ${PROJECT_SOURCE_DIR}/include/MainWindow.h ${PROJECT_SOURCE_DIR}/include/GripSimulation.h)
file(GLOB GUI_SRC include/*.h src/*.cpp )
add_library(mainWindow ${GUI_MOC_HEADERS} ${GUI_SRC})
target_link_libraries(mainWindow ${project_libs} ${DART_LIBRARIES} ${X11_THREADS_LIB})

# Set project libraries variable
set(project_libs ${project_libs} mainWindow)

#-----------------------------------------------
#                BUILD GRIP
#-----------------------------------------------
add_executable(grip ${PROJECT_SOURCE_DIR}/src/grip.cpp)
target_link_libraries(grip ${project_libs} ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})
set_target_properties( grip PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

# Added GripObjects here to avoid linking errors
FILE(GLOB grip-srcs osgGolems/*.h osgGolems/src/*.cpp osgDart/*.h osgDart/src/*.cpp qtWidgets/*.h qtWidgets/src/*.cpp src/GripObjects.cpp )
add_library(grip-core SHARED ${grip-srcs})
target_link_libraries(grip-core  ${QT_LIBRARIES} ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})
set_target_properties( grip-core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

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
INSTALL(PROGRAMS bin/grip
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

# Install library of grips libraries
INSTALL(TARGETS grip-core
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )

# Install header files by namespace and directory
FILE(GLOB osgDartHeaders "osgDart/*.h")
INSTALL(FILES ${osgDartHeaders}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/grip/osgDart )

FILE(GLOB osgGolemsHeaders "osgGolems/*.h")
INSTALL(FILES ${osgGolemsHeaders}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/grip/osgGolems )

FILE(GLOB qtWidgetsHeaders "qtWidgets/*.h")
INSTALL(FILES ${qtWidgetsHeaders}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/grip/qtWidgets)

FILE(GLOB mainHeaders "include/*.h")
INSTALL(FILES ${mainHeaders}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/grip)

# Install cmake find file for grip
INSTALL(FILES cmake/FindGRIP.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake )

# Install desktop file to call grip from desktop
INSTALL(FILES "doc/grip.desktop"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

INSTALL(FILES "src/icons/grip-icon.png"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/)


