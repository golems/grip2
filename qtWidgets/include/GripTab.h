#ifndef GRIPTAB_H
#define GRIPTAB_H

#include "TreeViewReturn.h"
#include "../../osgGolems/include/ViewerWidget.h"
#include <QDockWidget>
#include <QtPlugin>


/**
 * \class GripTab GripTab.h
 * \brief This class is used as an interface for defining plugins
 * The aim is to ensure that all methods declared in this class are defined in the plugins
 */
class GripTab : public QDockWidget
{
protected:
    /// used to manipulate the objects in the main window
    /// pointer to the object selected in the Tree View
    TreeViewReturn* activeNode;

    /// pointer to the osg viewer
    ViewerWidget* viewWidget;

public:
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
};

Q_DECLARE_INTERFACE(GripTab,
                    "com.gatech.Grip2.GripTab/1.0")

#endif // GRIPTAB_H
