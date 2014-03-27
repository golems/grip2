find_path(GRIP_INCLUDE_DIR grip/qtWidgets/GripTab.h
          PATHS "${CMAKE_INSTALL_PREFIX}/include")
set(GRIP_LIBRARIES grip-core
    PATHS "${CMAKE_INSTALL_PREFIX}/lib")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Grip DEFAULT_MSG
                                  GRIP_INCLUDE_DIR GRIP_LIBRARIES)

mark_as_advanced(GRIP_INCLUDE_DIR GRIP_LIBRARIES)