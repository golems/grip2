1. Download and install grip from https://github.com/golems/grip2
2. Build grip using the commands
	mkdir build
	cd build/
	cmake ..
	make
	make install

3. Create a project for the plugin that you want to create
4. In the CMakeLists.txt include the following

	find_package(GRIP REQUIRED)
	include_directories(${GRIP_INCLUDE_DIRS})
	
	add_executable(foo foo.cc)
	target_link_libraries(foo ${GRIP_LIBRARIES})
	
Note: Remember that the plugin needs to compile into a library and not an executable file. Hence include the cmake commands to generate a library from your code. While creating the library add the GripTab.h file to the library else when the library is being executed it will not understand the reference to GripTab.
	
5. Create a UI file using QT Designer that is of type QDockWidget and add the UI elements as needed
6. In the header files add the following includes
	#include <grip/qtWidgets/GripTab.h>
	#include <grip/qtWidgets/ViewerWidget.h>

7. The plugin class has to implement the GripTab interface in grip. Therefore you should change the inheritance from QDockWidget to GripTab
8. The class definition should define the object as a Q_OBJECT and should inform that it is implementing an interface. A sample structure would be:
	class PointCloudPlugin : public GripTab
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
	
9. The plugin has access to the following objects from the grip implementation
	/// used to manipulate the objects in the main window
	/// pointer to the object selected in the Tree View
	TreeViewReturn* activeNode;

	/// pointer to the osg viewer
	ViewerWidget* viewWidget;
   You can use these data structures to know the selected item from the objet explorer (tree view) and mnipulate or add osg objects into the rendered simualtion
   
10. The plugin has to implement the following methods though the implementation could be blank.
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

11. The plugin needs to be compiled into a library and placed in the plugin folder inside grip. If the plugin conforms to the GripTab structure, it will be loaded at runtime.
