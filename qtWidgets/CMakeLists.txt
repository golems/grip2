cmake_minimum_required(VERSION 2.8.3)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
if(Qt4_NOT_FOUND)
    find_package(Qt4 COMPONENTS QtCore QtGui Qt3Support QtXml REQUIRED)
    add_definitions(${QT_DEFINITIONS})
    include(${QT_USE_FILE})
endif()

# Add include directory to include_directories variable
include_directories(include "../osgDart/include" "../osgNodes/include")

# Add include directory in order to find ui_*.h files
include_directories(${CMAKE_BINARY_DIR}/qtWidgets)

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
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)

# Add Qt Widgets library
add_library(qtWidgets ${srcs} ${UI} ${MOC} ${RESOURCES})
target_link_libraries(qtWidgets  ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
