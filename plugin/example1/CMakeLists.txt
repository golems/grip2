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
# # Find QT
# find_package(Qt4 COMPONENTS QtCore QtGui Qt3Support QtXml REQUIRED)
# add_definitions(${QT_DEFINITIONS})
# ADD_DEFINITIONS(-DQT_PLUGIN)
# ADD_DEFINITIONS(-DQT_NO_DEBUG)
# ADD_DEFINITIONS(-DQT_SHARED)
# include(${QT_USE_FILE})
# Find QT
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL QtXml REQUIRED) # Qt3Support
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

# # Find OpenSceneGraph
# find_package(OpenSceneGraph 2.8 REQUIRED
#     COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
# include_directories(${OpenSceneGraph_INCLUDE_DIRS})
# add_definitions(${OpenSourceGraph_DEFINITIONS})
# set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)
# Try to find the OpenSceneGraph cmake package 
find_package(OpenSceneGraph 3.0 QUIET
    COMPONENTS osg osgViewer osgManipulator osgGA osgDB)
find_package(OpenSceneGraph 3.0 QUIET COMPONENTS osgQt)
if(${OpenSceneGraph_FOUND})
    message("Found OpenSceneGraph cmake package.")
    include_directories(${OpenSceneGraph_INCLUDE_DIRS})
    add_definitions(${OpenSourceGraph_DEFINITIONS})
    set(OPENSCENEGRAPH_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} osgQt)
else(${OpenSceneGraph_FOUND})
    message("OpenSceneGraph cmake package not found.  Searching for library...")
    find_library(OpenSceneGraph REQUIRED
        COMPONENTS osg osgViewer osgManipulator osgGA osgDB osgQt)
    if(${OpenSceneGraph-NOTFOUND})
        message("OpenSceneGraph library not found!")
    else(${OpenSceneGraph-NOTFOUND})
        message("Found installed OpenSceneGraph library.")
    endif()
    if(APPLE)
        set(OSG_LIBRARY_PATH "/usr/local/lib") # works on OSX 10.9.4
        set(LIB_SUFFIX "dylib")
    else(APPLE)
        # set(OSG_LIBRARY_PATH "/usr/local/lib64") # works on ubuntu 14.04, src install
        set(OSG_LIBRARY_PATH "/usr/lib") # works on ubuntu 14.04, apt-get install
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
endif()

# # Find Point Cloud Library
# find_package(PCL 1.3 REQUIRED COMPONENTS common io)
# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})
# add_definitions(${PCL_DEFINITIONS})

# Find DART
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODLUE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
find_package(DART REQUIRED)
include_directories(${DART_INCLUDE_DIRS})
link_directories(${DART_LIBRARY_DIRS})

# Find GRIP (system install; FindGRIP.cmake currently lives here:)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/share/cmake)
find_package(GRIP REQUIRED)
include_directories(${GRIP_INCLUDE_DIRS})

#-----------------------------------------------
#      INCLUDES & SUBDIRECTORY LIBRARIRES
#-----------------------------------------------
# Include top-level include directory
include_directories(include)
include_directories(${EXECUTABLE_OUTPUT_PATH})
include_directories(${CMAKE_BINARY_DIR}/plugin/example1)

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
target_link_libraries(myplugin  ${QT_LIBRARIES} ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${DART_LIBRARIES} ${GRIP_LIBRARIES})

# Set project libraries variable
set(project_libs myplugin)
