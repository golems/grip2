project( osgDart )

# Include include directory
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/osgGolems)


#-----------------------------------------------
#                CREATE LIBRARY
#-----------------------------------------------
# DART OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library(osgDart SHARED ${srcs})
target_link_libraries(osgDart ${DART_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})

message( STATUS "OUTPUT DIRECTORY MESSING IS: ${CMAKE_SOURCE_DIR} ")
set_target_properties( osgDart PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
