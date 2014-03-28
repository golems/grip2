grip2
=====

Qt and OpenSceneGraph user interface for grip, currently being centered around DART (Dynamic Animation Robotics Toolkit), but being designed with modularity in mind in order to be useful to external kinematics and dynamics libraries.


Overview
========

  1. qtWidgets

  2. osgDart

  3. osgGolems

Dependencies
============

The current dependencies are the following. 
      
  - Eigen3 (libeigen3-dev)
  - OpenSceneGraph (libopenscenegraph-dev)
  - Qt4 (libqt4-dev)
  - DART (libdart3.0, libdart-core3.0)

These can all be installed using
  
    sudo apt-get install <package_name>

Compiling
=========

    mkdir build
    cd build
    cmake ..
    make

Installation
============

  After compiling run
  
    sudo make install

Debugging
=========

  This section assumes you are in the build directory for all commands.

  Running cmake in debug mode defines the preprocessor variable "DEBUG_BUILD".
  In the top of some implementation files (.cpp) there exist the following code,
  which allows a developer to call the funciton DEBUG() as follows, for example:
  
      DEBUG("Error code: " << error_code);
  
  which is the same as writing
  
      std::cerr << "Error code: " << error_code << std::endl;
      
  Any function call to DEBUG will only print out if the package is compiled in "Debug" mode.
  The build mode is defined in the top-level CMakeLists.txt file, but can be set when running
  cmake using the following command:
  
      cmake .. -DCMAKE_BUILD_TYPE=Debug
      
  To set the build mode back to release just type
  
      cmake .. -DCMAKE_BUILD_TYPE=Release
      
  If there is any problem with cmake or make after making major changes such as filename changes, etc.,
  you can clear cmake's cache of variables, etc. by deleting the CMakeCache.txt file and rerunning cmake:
  
      rm CMakeCache.txt
      cmake ..

Documentation
=============

  1. Generating Documentation

    HTML documentation can be generated using Doxygen (http://www.stack.nl/~dimitri/doxygen/).
    There is a Doxyfile configuration file in the "doc" directory. You can use doxygen to parse the file
    and generate documentation for the entire package as follows:
  
        cd doc
        doxygen Doxyfile
      
    This generates an index.html file in the newly created html directory. Open this in any browser to view
    the documentation.
  
  2. Documentation Style in header files
  
    Comment blocks
    
        /**
         * \brief example
         */
       
    One-line comments
    
        ///
        int example
      
        int example ///< Comment
