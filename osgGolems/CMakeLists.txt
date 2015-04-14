project( osgGolems )

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------

include_directories(./)

# OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library(osgGolems SHARED ${srcs})
target_link_libraries(osgGolems ${OPENSCENEGRAPH_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL )
set_target_properties( osgGolems PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
