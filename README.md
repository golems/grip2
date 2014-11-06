grip2
=====

Qt and OpenSceneGraph user interface for grip, currently being centered around DART (Dynamic Animation Robotics Toolkit), but being designed with modularity in mind in order to be useful to external kinematics and dynamics libraries.
Grip2 is designed to be used with user plugins. Plugins can be loaded via "File->Load Plugins" and selecting a directory with the user's plugin shared library files.

Directory Overview
==================

  1. grip

  grip contains everything that depends on DART, Qt, and OpenSceneGraph, including grip's main window, the visualization window and the simulation thread.

  2. qtWidgets

  Qt is used for the application level, including the interface, GUI event handling, etc.

  qtWidgets contains Qt widgets that only depend on Qt.

  3. osgDart

  osgDart defines the interface between OpenSceneGraph and DART. For every object in DART a node in OpenSceneGraph is created and can be rendered to an OpenSceneGraph View.

  4. osgGolems

  osgGolem defines a set of libraries for OpenSceneGraph that only depend on OpenSceneGraph.

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

Running Grip2
=============

  After installing grip2, you can start grip2 up using one of the two methods below:

  - Open a terminal and type

    grip

  - Goto Dash Home by clicking on it in the Launcher or by pressing the Windows key, and search for "grip" or "Grip".


Debugging
=========

  Currently, each class that could require debugging has a private member variable, "_debug", which gets set by the parent that instantiates the class. 

  The default value should always be "false".  

Documentation
=============

  Documentation is located at http://golems.github.io/grip2/

  1. Generating Documentation

  HTML documentation can be generated using Doxygen (http://www.stack.nl/~dimitri/doxygen/).
  There is a Doxyfile configuration file in the "doc" directory. You can use doxygen to parse the file
  and generate documentation for the entire package as follows:

      cd doc
      doxygen Doxyfile

  This generates an index.html file in the newly created html directory. Open this in any browser to view
  the documentation.

  2. Documentation Style in header files. All functions should contain a "\brief" and "\return" comment and "\param" comments if applicable.

  Comment blocks

      /**
       * \brief Sets the radius of the circle to the value passed in by the user
       * \param newRadius The new radius for the circle
       * \return void
       */
      void setRadius(float newRadius);

  One-line comments

      /// The radius of the cylinder
      int _radus;

      int _height ///< Height of the cylinder

Coding Guidelines
=================

  1. Qt Coding Standard should be used for all source code in Grip2 except for the exceptions stated in section 2 following this section. Refer to the following references,

  http://qt-project.org/wiki/Coding-Conventions

  http://qt-project.org/wiki/Qt_Coding_Style

  http://doc.qt.digia.com/qq/qq13-apis.html#theartofnaming

  2. Exceptions to Qt Coding Standard

  Class private and protected member variables and functions should have the prefix (_) (an underscore), like follows, to improve clarity when reading source code.

    <pre><code>int _classMember;
  void _classFunction();</code></pre>

  Case statements should be indented from switch statements with optional braces and case keywords should all line up vertically.
    
    <pre><code>switch (type) {
        case LINE: {
            line = 3;
            break;
        }
        case LINE_WITH_ARROW: {
            line = 4;
            break;
        }
        default: {
            break;
        }
  }</code></pre>

  3. If you can't find out the correct way to do something, refer to existing source code in the project.

Creating Grip2 Plugins
======================
1. Download and install grip 
2. Create a project for the plugin that you want to create
3. In the CMakeLists.txt include the following
<pre><code>find_package(GRIP REQUIRED)
	include_directories(${GRIP_INCLUDE_DIRS})
	
	add_library(foo SHARED ${SRCS} ${UI} ${MOC} ${RESOURCES})
	target_link_libraries(foo ${QT_LIBRARIES} ${DART_LIBRARIES}${GRIP_LIBRARIES})
</code></pre>
Note: Remember that the plugin needs to compile into a library and not an executable file. Hence include the cmake commands to generate a library from your code. While creating the library add the GripTab.h file to the library else when the library is being executed it will not understand the reference to GripTab.
	
5. Create a UI file using QT Designer that is of type QDockWidget and add the UI elements as needed
6. In the header files add the following includes
<pre><code>#include &lt;grip/qtWidgets/GripTab.h&gt;
	#include &lt;grip/qtWidgets/ViewerWidget.h&gt;
</code></pre>
7. The plugin class has to implement the GripTab interface in grip. Therefore you should change the inheritance from QDockWidget to GripTab
8. The class definition should define the object as a Q_OBJECT and should inform that it is implementing an interface. A sample structure would be:
<pre><code>class Plugin_Name : public GripTab
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
<pre><code>/// used to manipulate the objects in the main window
	/// pointer to the object selected in the Tree View
	TreeViewReturn* activeNode;

	/// pointer to the osg viewer
	ViewerWidget* viewWidget;
</code></pre>
   You can use these data structures to know the selected item from the objet explorer (tree view) and mnipulate or add osg objects into the rendered simualtion
   
10. GripTab has the following list of pure virtual functions that need to be implemented in your plugin.The plugin can implement the following methods. Note that Refresh is a pure virtual function and has to be implemented in the plugin even if the implementation is blank.


<pre><code>/**
 * \brief called from the main window whenever a new scene is loaded
 */
virtual void Refresh() = 0;

/**
 * \brief called from the main window whenever the plugin is added to grip
 * This is initalize the members of the class
 * \param ret Pointer to object returned by the TreeView
 * \param viewer Pointer to composite viewer object where things are rendered
 * \param world Pointer to the dart world simulation object
 * \param timeline Array of GripTimeslice object for simulation and kinematic playback
 */
virtual void Load(TreeViewReturn *ret,
                  ViewerWidget *viewer,
                  dart::simulation::World *world,
                  std::vector<GripTimeslice> *timeline)

/**
 * \brief called from the main window whenever a new scene file is loaded
 */
virtual void GRIPEventSceneLoaded(){}

/**
 * \brief called from the main window whenever the simulation is executing
 * This method is executed before every simulation time step
 */
virtual void GRIPEventSimulationBeforeTimestep(){}

/**
 * \brief called from the main window whenever the simulation is executing
 * This method is executed after every simulation time step
 */
virtual void GRIPEventSimulationAfterTimestep(){}

/**
 * \brief called from the main window whenever the simulation is executing
 * This method is executed at the start of the simulation
 */
virtual void GRIPEventSimulationStart(){}

/**
 * \brief called from the main window whenever the simulation is executing
 * This method is executed at the end of the simulation
 */
virtual void GRIPEventSimulationStop(){}


/**
 * \brief called from the main window whenever the simulation history slider is being played
 * This method is executed before every playback time step
 */
virtual void GRIPEventPlaybackBeforeFrame(){}

/**
 * \brief called from the main window whenever the simulation history slider is being played
 * This method is executed after every playback time step
 */
virtual void GRIPEventPlaybackAfterFrame(){}

/**
 * \brief called from the main window whenever the simulation history slider is being played
 * This method is executed at the start of the playback
 */
virtual void GRIPEventPlaybackStart(){}

/**
 * \brief called from the main window whenever the simulation history slider is being played
 * This method is executed at the end of the playback
 */
virtual void GRIPEventPlaybackStop(){}

/**
 * \brief called from the main window when a new object is selected in the treeview
 */
virtual void GRIPEventTreeViewSelectionChanged(){}</code></pre>

Once complete the plugin needs to be compiled into a library. If the plugin conforms to the GripTab structure, it will be loaded whenever the user selects the library file to be loaded from the grip menu.
