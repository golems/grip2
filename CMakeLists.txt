CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(grip2)

FIND_PACKAGE(Qt4 REQUIRED)


# setting aliases for the various subfolders in the source
SET(GRIP2_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(GRIP2_GUI_DIR ${GRIP2_SRC_DIR}/GUI)

#including the various subfolders defined above
include_directories(${GRIP2_SRC_DIR} ${GRIP2_GUI_DIR})

# naming the files in the subfolders
SET(GUI_SRC ${GRIP2_GUI_DIR}/mainwindow.cpp)
SET(GUI_HEADERS ${GRIP2_GUI_DIR}/mainwindow.h)

#creating MOC files
QT4_WRAP_CPP(GUI_HEADERS_MOC ${GUI_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_EXECUTABLE(grip2 main.cpp ${GUI_SRC} ${GUI_HEADERS_MOC})

TARGET_LINK_LIBRARIES(grip2 ${QT_LIBRARIES})
