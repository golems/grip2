cmake_minimum_required(VERSION 2.8.11)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
find_package( Qt5Core )
find_package( Qt5Gui )
find_package( Qt5Xml )
find_package( Qt5OpenGL )

# Add include directory to include_directories variable
include_directories(./ "../osgDart/" "../osgGolems/")

# Add include directory in order to find ui_*.h files
include_directories(${CMAKE_BINARY_DIR}/qtWidgets)

# Wrap .ui files
FILE(GLOB UI_FILES ./ui/*.ui)
QT5_WRAP_UI(UI ${UI_FILES})

# Wrap .moc files, but remove files with no implementation
FILE(GLOB MOC_FILES ./*.h)
LIST(REMOVE_ITEM MOC_FILES "${CMAKE_CURRENT_LIST_DIR}/./GripTab.h"
                           "${CMAKE_CURRENT_LIST_DIR}/./TreeViewReturn.h"
                           "${CMAKE_CURRENT_LIST_DIR}/./Plotting.h")
QT5_WRAP_CPP(MOC ${MOC_FILES})

# Add .qrc files
file(GLOB RC_FILES *.qrc)
QT5_ADD_RESOURCES(RESOURCES ${RC_FILES})

# Add source and header files
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)

# Add Qt Widgets library as SHARED library to dynamically load icons during runtime
add_library(qtWidgets SHARED ${srcs} ${UI} ${MOC} ${RESOURCES})
target_link_libraries(qtWidgets osgDart osgGolems ${OPENSCENEGRAPH_LIBRARIES} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml ${DART_LIBRARIES} )
set_target_properties( qtWidgets PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
