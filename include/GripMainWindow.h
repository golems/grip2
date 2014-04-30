/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Shailesh Lohia <shailesh.lohia@gatech.edu>
 * Date: Jan 2014
 *
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of the Humanoid Robotics Lab nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file GripMainWindow.h
 * \brief Main window for Grip
 */

#ifndef GRIPMAINWINDOW_H
#define GRIPMAINWINDOW_H

// C++ Standard includes
#include <iostream>
#include <cstdio>

// Base class include
#include "MainWindow.h"

// Local includes
#include "ViewerWidget.h"
#include "TreeView.h"
#include "InspectorTab.h"
#include "VisualizationTab.h"
#include "PlaybackWidget.h"
#include "ui_VisualizationTab.h"
#include "ui_InspectorTab.h"
#include "ui_TreeView.h"
#include "ui_PlaybackWidget.h"
#include "DartNode.h"
#include "GripSimulation.h"
#include "GripTab.h"
#include "GripTimeslice.h"
#include "GripObjects.h"

// Qt includes
#include <QDir>
#include <QtXml>

/**
 * \class GripMainWindow GripMainWindow.h
 * \brief Class that subclasses MainWindow in order to create the whole
 * Grip interface.
 */
class GripMainWindow : public MainWindow, public GripObjects
{
public:
    /**
     * \brief Constructs a GripMainWindow object
     * \param debug Whether or not to print debug statements. This gets passed to
     * local class objects
     * \param sceneFile String representing the scene file to load on startup. If empty or
     * unspecified, a scene will not be loaded.
     * \param configFile String representing the workspace file to load on startup. If empty or
     * unspecified, a workspace will not be loaded.
     */
    GripMainWindow(bool debug=false, std::string sceneFile="", std::string configFile="");

    /**
     * \brief Destructs a GripMainWindow object
     */
    ~GripMainWindow();

    /**
     * \brief Convenience function for creating a ground skeleton
     * \return Ground object
     */
    dart::dynamics::Skeleton *createGround();

//    GripObjects *grip;

    /// OpenSceneGraph Qt composite viewer widget, which can hold more than one view
//    ViewerWidget *viewWidget;
    ViewerWidget *recordViewWidget;

    /// QDockWidget that contains a QTreeWidget. It is used as an object explorer for the loaded skeletons or robots
    TreeView *treeviewer;

    /// Tab for manually manipulating objects in the world
    InspectorTab *inspectorTab;

    /// Tab for changing visualization settings of the render window
    VisualizationTab *visualizationTab;

    /// Array of GripTimeSlice objects stored for simulation/kinematic playback
//    std::vector<GripTimeslice> *timeline;
    
    /// Widget for playing back the simulation or kinematic states in the timeline
    PlaybackWidget *playbackWidget;

    /// TreeViewReturn class is defined in tree_view.h
    /// It contains two members: void* object and DataType dataType
    /// void* object can store a dart Skeleton object or BodyNode object
    /// DataType is an enumaration defined in tree_view.h which can take
    /// the values Return_Type_Robot and Return_Type_Node
//    TreeViewReturn *activeItem;

    /// The world object that is being rendered and simulated
//    dart::simulation::World *world;

    /// Main OpenSceneGraph node for the main view of the composite viewer
    osgDart::DartNode *worldNode;

    /// Simulation thread doing the actually simluation loop
    GripSimulation *simulation;

    /// stores a list of images to be saved as videos
    QList<QImage>* recordImageList;

public slots:
    /**
     * \brief Slot for setting the state and time of the world based on
     * the playback widget's slider position and the contents of the timeline
     * \param sliderTick Position of the playback widget's slider
     */
    void slotSetWorldFromPlayback(int sliderTick);

    /**
     * \brief Slot for starting the playback
     */
    void slotPlaybackStart();

    /**
     * \brief Slot for pausing the playback
     */
    void slotPlaybackPause();

    /**
     * \brief Slot for reversing the playback
     */
    void slotPlaybackReverse();

    /**
     * \brief Slot for going to the beginning of the playback timeline
     */
    void slotPlaybackBeginning();

    /**
     * \brief Slot that recursively calls it self to run the playback loop.
     * The recursive call allows the event loop to process other events, (eg. Stop Playback)
     * \param playForward Whether or not to play back in forward or reverse
     */
    void slotPlaybackTimeStep(bool playForward);

protected slots:

    /**
     * \brief Sets the time box for simulation time relative to real time
     * \param time The simulation time relative to real time
     * \return void
     */
    void setSimulationRelativeTime(double time);

    /**
     * \brief Set the view to front view
     * \return void
     */
    void front();

    /**
     * \brief Set the view to top view
     * \return void
     */
    void top();

    /**
     * \brief Set the view to side view
     * \return void
     */
    void side();

    /**
     * \brief Starts the simulation
     * \return void
     */
    void startSimulation();

    /**
     * \brief Stops the simulation
     * \return void
     */
    void stopSimulation();

    /**
     * \brief Simulates a single time step of the simulation
     * \return void
     */
    void simulateSingleStep();

    /**
     * \brief Turns on rendering during simulation, which will simulation slower than if
     * rendering is turned off.
     */
    void renderDuringSimulation();

    /**
     * \brief Sets the background color of the window to white
     * \return void
     */
    void white();

    /**
     * \brief Sets the background color of the window to gray
     * \return void
     */
    void gray();

    /**
     * \brief Sets the background color of the window to black
     * \return void
     */
    void black();

    /**
     * \brief Resets the camera position to a position so that you can see the front view of the rendered objects
     * \return void
     */
    void resetCamera();

    /**
     * \brief Change movie export mode to 1024x768
     */
    void xga1024x768();

    /**
     * \brief Change movie export mode to 640x480
     */
    void vga640x480();

    /**
     * \brief Change movie export mode to 1280x720
     */
    void hd1280x720();

    /**
     * \brief Takes a screenshot of the rendering window
     */
    void camera();

    /**
     * \brief Takes a series of screenshots of the rendering window while it is playing back a simulation
     */
    void film();

    /**
     * \brief Close current scene
     */
    void close();

    /**
     * \brief Notifies thread that simulation has stopped
     * \return void
     */
    void simulationStopped();

protected:
    /** Protected Members */
    /// Any plugin that is loaded successfully into the Grip will get stored in this QList
    /// The plugins are always going to be derived from the GripTab interface defined in qtWidgets/include/GripTab.h
    /// In order to execute one of the pure virtual functions defined in the GripTab declaration,
    /// you first need to perform a qobject_cast. An example for the same is
    /// GripTab* gt = qobject_cast<GripTab*>(plugin);
    /// Once a pointer of type GripTab is created you can then call the function directly.
    QList<GripTab*> *pluginList;

    /// Plugin menu pointer for showing/hiding plugins
	QMenu *pluginMenu;

    /// Stores the path to all the plugins that are loaded
    QList<QString*>* pluginPathList;

    /// Stores the path to the scene file
    QString* sceneFilePath;

    /// Stores the height width resolution for the recording
    QSize recordSize;

    /// Pointer to the widget that run the simulation playback for recording
    QWidget* recordWidget;

    /**
     * \brief Create an XML file for the workspace
     * contains the list of plugins, status of DockWidgets and the loaded scene
     * \return QDomDocument
     */
    QDomDocument* generateWorkspaceXML();

    /**
     * \brief Parses the configuration file and manipulates the workspace to match the settings in the configuration file
     * \return void
     */
    void parseConfig(QDomDocument config);

    /**
     * \brief Creates the rendering window
     * \return void
     */
    void createRenderingWindow();

    /**
     * \brief Creates the tree view
     * \return void
     */
    void createTreeView();

    /**
     * \brief Creates the default tabs
     * \return void
     */
    void createTabs();

    /**
     * \brief Manage layout of the main window
     * \return void
     */
    void manageLayout();

    /**
     * \brief creates the Plugin Menu
     * \return void
     */
    void createPluginMenu();

    /**
     * \brief Clears the world, simulation and widgets
     * \return void
     */
    void clear();

    /**
     * \brief Pops up a dialog box for the user to confirm if he wants to stop the
     * currently running simulation.
     * \return bool Whether or not the user want to stop the simulation or not
     */
    bool stopSimulationWithDialog();

    /**
     * \brief Swaps the start and stop buttons for covenience. It just shows and hides them.
     * \return void
     */
    void swapStartStopButtons();

    /**
     * \brief Load the scene and renders it. This function resets everything
     * on each load.
     * \param fileName Name of scene file to load
     * \return void
     */
    void doLoad(std::string sceneFileName);

    /**
     * \brief Saves the loaded scene to file for quick load functionality
     * \return void
     */
    int saveText(std::string scenepath, const QString &filename);


    /**
     * \brief Reads a QDir directory and loads all the plugins inside it
     * \param pluginsDirName Name of the QDir containing the plugins
     */
    void loadPluginDirectory(QDir pluginsDirName);

    /**
     * \brief Loads a plugin file (.so) by name. When loaded, the plugin will be
     * added to the bottom tabs area.
     * \param pluginFileName String representing the path of the plugin
     */
    void loadPluginFile(QString pluginFileName);

    /**
     * \brief Set state of world from an Eigen::VectorXd.
     * Because DART has a major bug with the integration of free and ball
     * joints, some extra work arounds have to been performed.
     * \param newState New state for the world
     */
    void setWorldState_Issue122(const Eigen::VectorXd &newState);

    /**
     * \brief Records playback images
     * \param QList<QImage>* to list of QImages where the images are stored
     * \param ViewerWidget* pointer to osg viewer object whose images are stored
     * \return void
     */
    void recordPlayback(QList<QImage>* imageList, ViewerWidget* vWidget);

    /**
     * \brief saves video
     * \param QList<QImage>*
     * \return void
     */
    void saveVideo();

    /// used to maintain the layout of the widgets that are not QDockWidgets
    QGridLayout *gridLayout;

    bool _debug;            ///< Whether or not to print debug statements
    bool _simulating;       ///< Whether or not simulation is happening
    bool _playingBack;      ///< Whether or not playback is happening
    size_t _curPlaybackTick;   ///< Current tick in playback
    size_t _playbackSpeed;     ///< Additive value corresponding to playback speed
    bool _simulationDirty;  ///< Whether or not the timeline has been messed with
    bool _recordVideo;      ///< Whether or not to store viewWidget images>
};


#endif // GRIPMAINWINDOW_H
