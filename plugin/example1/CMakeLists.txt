cmake_minimum_required(VERSION 2.8.3)
project(myplugin)

#-----------------------------------------------
#                COMPILE SETTINGS
#-----------------------------------------------
set(CMAKE_BUILD_TYPE "Debug")
set(BUILD_FLAGS "-std=gnu++0x -Wreturn-type -Wno-enum-compare -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Define preprocessor macro for Debug printing in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in \"Debug\" mode. Defining DEBUG_BUILD macro.")
    add_definitions(-DDEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in \"Release\" mode.")
else()
endif()

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui Qt3Support QtXml REQUIRED)
add_definitions(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
include(${QT_USE_FILE})

# Find OpenSceneGraph
find_package(OpenSceneGraph 2.8 REQUIRED
    COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})
add_definitions(${OpenSourceGraph_DEFINITIONS})
set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)

# Find Point Cloud Library
find_package(PCL 1.3 REQUIRED COMPONENTS common io)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(GRIP REQUIRED)
include_directories(${GRIP_INCLUDE_DIRS})

#-----------------------------------------------
#      INCLUDES & SUBDIRECTORY LIBRARIRES
#-----------------------------------------------
# Include top-level include directory
include_directories(include)
include_directories(${EXECUTABLE_OUTPUT_PATH})

# Add include directory in order to find ui_*.h files
include_directories(${CMAKE_BINARY_DIR})

# Wrap .ui files
FILE(GLOB UI_FILES ./ui/*.ui)
QT4_WRAP_UI(UI ${UI_FILES})

# Wrap .moc files
FILE(GLOB MOC_FILES ./include/*.h)
QT4_WRAP_CPP(MOC ${MOC_FILES})

# Add .qrc files
file(GLOB RC_FILES .rc/*.qrc)
QT4_ADD_RESOURCES(RESOURCES ${RC_FILES})

# Add source and header files
#file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB SRCS include/*.h src/*.cpp)

# Add Qt Widgets library
add_library(myplugin SHARED ${SRCS} ${UI} ${MOC} ${RESOURCES})
target_link_libraries(myplugin  ${QT_LIBRARIES} ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${GRIP_LIBRARIES})

# Set project libraries variable
set(project_libs myplugin)
