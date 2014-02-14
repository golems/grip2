cmake_minimum_required(VERSION 2.8.3)

#-----------------------------------------------
#                FIND PACKAGES
#-----------------------------------------------
# Find QT
if(Qt4_NOT_FOUND)
    find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
    add_definitions(${QT_DEFINITIONS})
    include(${QT_USE_FILE})
endif()

include_directories(include)

# OpenSceneGraph Nodes Library
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
add_library(qtWidgets ${srcs})
target_link_libraries(qtWidgets ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES})
