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
 * \file GripTab.h
 * \brief GripTab declares virtual functions and contains some useful member pointers to
 * objects in the main window of grip. Users making plugins need to subclass this class
 */

#ifndef GRIPTAB_H
#define GRIPTAB_H

// Local includes
#include "TreeViewReturn.h"
#include "../osgGolems/ViewerWidget.h"
#include "GripTimeslice.h"
#include "GripObjects.h"

// DART includes
#include <dart/simulation/World.h>

// Qt includes
#include <QDockWidget>
#include <QtPlugin>

class GripMainWindow;

/**
 * \class GripTab GripTab.h
 * \brief This class is used as an interface for defining plugins
 * The aim is to ensure that all methods declared in this class are defined in the plugins
 */
class GripTab : public QDockWidget
{
protected:
    /// pointer to the object selected in the Tree View
    TreeViewReturn *_activeNode;

    /// pointer to the osg viewer
    ViewerWidget *_viewWidget;

    /// pointer to simulation world object that is being rendered and simulated
    dart::simulation::World *_world;


    /// pointer to the timeline, which holds a GripTimeslice objects.
    /// These contain the state and time of the world. To use just call
    /// timeline->push_back(GripTimeslice(*world));
    std::vector<GripTimeslice> *_timeline;

    GripObjects *_grip;

public:
    /**
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
    virtual void Load(GripMainWindow *grip)
    {
        _grip = (GripObjects*)grip;
        _activeNode = _grip->getTreeViewReturn();
        _viewWidget = _grip->getViewerWidget();
        _world = _grip->getWorld();
        _timeline = _grip->getTimeline();
    }

    /**
     * \brief called when a new scene file (urdf, sdf) is loaded into Grip.
     * This signifies that there are skeletons in the world, the viewer is displaying
     * the objects in the world and the TreeView is populated
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
    virtual void GRIPEventTreeViewSelectionChanged(){}
};

Q_DECLARE_INTERFACE(GripTab,
                    "com.gatech.Grip2.GripTab/1.0")

#endif // GRIPTAB_H
