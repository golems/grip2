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

Creating Grip2 Plugins
=======================

1. Download and install grip from https://github.com/golems/grip2
2. Create a project for the plugin that you want to create
3. In the CMakeLists.txt include the following
<pre><code>
	find_package(GRIP REQUIRED)
	include_directories(${GRIP_INCLUDE_DIRS})
	
	add_executable(foo foo.cc)
	target_link_libraries(foo ${GRIP_LIBRARIES})
</code></pre>
    Note: Remember that the plugin needs to compile into a library and not an executable file. Hence include the cmakecommands to generate a library from your code. While creating the library add the GripTab.h file to the library else when the library is being executed it will not understand the reference to GripTab.
	
5. Create a UI file using QT Designer that is of type QDockWidget and add the UI elements as needed
6. In the header files add the following includes
<pre><code>
	#include &lt;grip/qtWidgets/GripTab.h&gt;
	#include &lt;grip/qtWidgets/ViewerWidget.h&gt;
</code></pre>
7. The plugin class has to implement the GripTab interface in grip. Therefore you should change the inheritance from QDockWidget to GripTab
8. The class definition should define the object as a Q_OBJECT and should inform that it is implementing an interface. A sample structure would be:
<pre><code>
	class Plugin_Name : public GripTab
	{
	    Q_OBJECT
	    Q_INTERFACES(GripTab)
	    
	    public:
	    void Load(TreeViewReturn* ret, ViewerWidget* viewer);
	    void GRIPEventSimulationBeforeTimestep();
	    void GRIPEventSimulationAfterTimestep();
	    void GRIPEventSimulationStart();
	    void GRIPEventSimulationStop();
	    void GRIPEventPlaybackBeforeFrame();
	    void GRIPEventPlaybackAfterFrame();
	    void GRIPEventPlaybackStart();
	    void GRIPEventPlaybackStop();
	    void GRIPEventTreeViewSelectionChanged();
	    void Refresh();
	    /* other public members and methods */
	    protected:
	    /* protected members and methods */
	    private:
	    /* private members and methods */
	    public slots:
	    /* slots corresponding to the ui elements*/
	}
</code></pre>
9. The plugin has access to the following objects from the grip implementation
<pre><code>
	/// used to manipulate the objects in the main window
	/// pointer to the object selected in the Tree View
	TreeViewReturn* activeNode;

	/// pointer to the osg viewer
	ViewerWidget* viewWidget;
</code></pre>
   You can use these data structures to know the selected item from the objet explorer (tree view) and mnipulate or add osg objects into the rendered simualtion
   
10. GripTab has the following list of pure virtual functions that need to be implemented in your plugin.The plugin has to implement the following methods though the implementation could be blank.


    <pre><code>
    /**
     * \brief called from the main window whenever the model changes
     */
    virtual void Refresh() = 0;
    /**
     * \brief called from the main window whenever the plugin is added to grip
     * This is initalize the members of the class
     */
    virtual void Load(TreeViewReturn* ret, ViewerWidget* viewer) = 0;
    /**
     * \brief called from the main window whenever the simulation is executing
     * This method is executed before every simulation time step
     */
    virtual void GRIPEventSimulationBeforeTimestep() = 0;
    /**
     * \brief called from the main window whenever the simulation is executing
     * This method is executed after every simulation time step
     */
    virtual void GRIPEventSimulationAfterTimestep() = 0;
    /**
     * \brief called from the main window whenever the simulation is executing
     * This method is executed at the start of the simulation
     */
    virtual void GRIPEventSimulationStart() = 0;
    /**
     * \brief called from the main window whenever the simulation is executing
     * This method is executed at the end of the simulation
     */
    virtual void GRIPEventSimulationStop() = 0;
    /**
     * \brief called from the main window whenever the simulation history slider is being played
     * This method is executed before every playback time step
     */
    virtual void GRIPEventPlaybackBeforeFrame() = 0;
    /**
     * \brief called from the main window whenever the simulation history slider is being played
     * This method is executed after every playback time step
     */
    virtual void GRIPEventPlaybackAfterFrame() = 0;
    /**
     * \brief called from the main window whenever the simulation history slider is being played
     * This method is executed at the start of the playback
     */
    virtual void GRIPEventPlaybackStart() = 0;
    /**
     * \brief called from the main window whenever the simulation history slider is being played
     * This method is executed at the end of the playback
     */
    virtual void GRIPEventPlaybackStop() = 0;
    /**
     * \brief called from the main window when a new object is selected in the treeview
     */
    virtual void GRIPEventTreeViewSelectionChanged() = 0;
    </code></pre>


11. The plugin needs to be compiled into a library and placed in the plugin folder inside grip. If the plugin conforms to the GripTab structure, it will be loaded at runtime.
