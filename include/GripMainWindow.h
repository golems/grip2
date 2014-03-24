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
#ifndef GRIPMAINWINDOW_H
#define GRIPMAINWINDOW_H


// C++ Standard includes
#include <iostream>
#include <cstdio>

// Base class include
#include "MainWindow.h"

#include <ViewerWidget.h>
#include <TreeView.h>

#include <inspector_tab.h>
#include <visualization_tab.h>
#include <time_display.h>
#include "ui_visualization_tab.h"
#include "ui_inspector_tab.h"

#include "ui_TreeView.h"
#include "GripTab.h"
#include "ui_time_display.h"

#include "DartNode.h"
#include "GripSimulation.h"

using namespace std;

/**
 * \class GripMainWindow GripMainWindow.h
 * \brief Class that subclasses MainWindow in order to create the whole
 * Grip interface.
 */
class GripMainWindow : public MainWindow
{

public:
    /**
     * \brief Constructs a GripMainWindow object
     */
    GripMainWindow();

    /**
     * \brief Destructs a GripMainWindow object
     */
    ~GripMainWindow();

    /**
     * \brief Convenience function for creating a ground skeleton
     * \return Ground object
     */
    dart::dynamics::Skeleton* createGround();

    /// OpenSceneGraph Qt composite viewer widget, which can hold more than one view
    ViewerWidget* viewWidget;

    /// QDockWidget that contains a QTreeWidget. It is used as an object explorer for the loaded skeletons or robots
    TreeView* treeviewer;

    /// Tab for manually manipulating objects in the world
    Inspector_Tab* inspectortab;

    /// Tab for changing visualization settings of the render window
    Visualization_Tab* visualizationtab;

    /// Display for the simluation time and its time relative to real time
    Time_Display* simulation_time_display;

    /// TreeViewReturn class is defined in tree_view.h
    /// It contains two members: void* object and DataType dataType
    /// void* object can store a dart Skeleton object or BodyNode object
    /// DataType is an enumaration defined in tree_view.h which can take
    /// the values Return_Type_Robot and Return_Type_Node
    TreeViewReturn* activeItem;

    /// Main OpenSceneGraph node for the main view of the composite viewer
    osgDart::DartNode* worldNode;

    /// The world object that is being rendered and simulated
    dart::simulation::World* world;

    /// Simulation thread doing the actually simluation loop
    GripSimulation* simulation;

    /// Vector to hold timeslice objects for the slider and playback
    std::vector<GripTimeslice> timeline;

protected slots:
    /**
     * \brief Sets the time box for simulation time relative to real time
     * \param time The simulation time relative to real time
     * \return void
     */
    void setSimulationRelativeTime(double time);

private slots:
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

    void resetCamera();

    void xga1024x768();

    void vga640x480();

    void hd1280x720();

    /**
     * \brief Notifies thread that simulation has stopped
     * \return void
     */
    void simulationStopped();

    void slotAddTimesliceToTimeline(const GripTimeslice& timeslice);

private:
    /** Private Members */
    /// Any plugin that is loaded successfully into the Grip will get stored in this QList
    /// The plugins are always going to be derived from the GripTab interface defined in qtWidgets/include/GripTab.h
    /// In order to execute one of the pure virtual functions defined in the GripTab declaration,
    /// you first need to perform a qobject_cast. An example for the same is
    /// GripTab* gt = qobject_cast<GripTab*>(plugin);
    /// Once a pointer of type GripTab is created you can then call the function directly.
    QList<GripTab*>* pluginList;

    /** Private Methods */
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
     * \brief Creates the playback slider
     * \return void
     */
    void createPlaybackSliders();

    /**
     * \brief Creates the time display boxes
     * \return void
     */
    void createTimeDisplays();

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
    void doLoad(string fileName);


    /**
     * \brief Saves the loaded scene to file for quick load functionality
     * \return void
     */
    int saveText(string scenepath, const char* llfile);

    /**
     * \brief Bool for whether or not we are currently simulating
     * \return bool
     */
    bool _simulating;

    /// This function reads a folder by the name of 'plugin' in source directory
    /// The plugin directory needs to contain the library for the plugins to be loaded (.so files on Linux, .dll files on Windows)
    void loadPlugins();
};


#endif // GRIPMAINWINDOW_H
