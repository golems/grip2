# - Find Grip
# This module searches for the Grip core "grip-core" library and headers.
#
#===================================================
# This module defines the following output variables
#
#   GRIP_FOUND        - Whether or not Grip was found
#   GRIP_INCLUDE_DIRS - Where to find the headers
#   GRIP_LIBRARIES    - The Grip libraries
#
#===================================================
# Example Usage:
#
# find_package(GRIP REQUIRED)
# include_directories(${GRIP_INCLUDE_DIRS})
#
# add_executable(foo foo.cc)
# target_link_libraries(foo ${GRIP_LIBRARIES})
#

find_path(GRIP_INCLUDE_DIRS grip/qtWidgets/GripTab.h
          PATHS "${CMAKE_INSTALL_PREFIX}/include")

find_library(GRIP_LIBRARIES grip-core
             NAMES grip-core
             PATH_SUFFIXES lib64 lib
             PATHS "${CMAKE_INSTALL_PREFIX}/lib")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Grip DEFAULT_MSG
                                  GRIP_INCLUDE_DIRS GRIP_LIBRARIES)

mark_as_advanced(GRIP_INCLUDE_DIRS GRIP_LIBRARIES)