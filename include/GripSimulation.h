/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
 *
 * Humanoid skeletonics Lab      Georgia Institute of Technology
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
 * \file GripSimulation.h
 * \brief Class for running the simulation loop in its own thread.
 */

#ifndef GRIP_SIMULATION_H
#define GRIP_SIMULATION_H

// DART includes
#include <dart/simulation/World.h>

// Qt includes
#include <QObject>
#include <QMetaType>

//// Local includes
#include "MainWindow.h"
#include "GripTab.h"
#include "GripTimeslice.h"
#include "ViewerWidget.h"

class GripMainWindow;

/**
 * \class GripSimulation GripSimulation.h
 * \brief Class that handles the simulation loop, simulation timeline, timing
 * and communicates with the main GUI thread via slots and signals. This class
 * subclass QObject and is used by creating an instance of the class and then
 * moving it to a thread with the "moveToThread(QThread*)" function.
 */
class GripSimulation : public QObject
{
    /// Macro to create meta-object code for the signals and slots
    Q_OBJECT

public:

    /**
     * \brief Constructs a GripSimulation object
     * \param parent Pointer to the parent widget. Default is 0
     * \param debug Flag for whether or not to output debug statements
     */
    GripSimulation(dart::simulation::World* world, std::vector<GripTimeslice>* timeline,
                   QList<GripTab*>* pluginLinst, ViewerWidget* viewer, MainWindow *parent=0, bool debug=false);

    /**
     * \brief Destroys the GripSimulation object
     */
    ~GripSimulation();

    /**
     * \brief Set the world object for the simulation
     * \param world A pointer to the dart::simulation::World to be simulated
     * \return void
     */
    void setWorld(dart::simulation::World* world);

    /**
     * \brief Reset the GripSimulation object
     * \return void
     */
    void reset();

signals:
    /**
     * \brief Signal to tell parent widget that the simulation loop is done. This is
     * used to make sure the simulation loop is done using the world object before the
     * parent deletes it.
     * \return void
     */
    void simulationStoppedSignal();

    /**
     * \brief Signal to pass the time value of the simulation real time
     * \param simTimeRelToRealTimeInstantaneous The simluation time relative to real time
     * \return void
     */
    void signalRelTimeChanged(double simTimeRelToRealTimeInstantaneous);

    /**
     * \brief Signal to pass message to status bar in main window
     * \param msg QString message to display in status bar
     * \return void
     */
    void signalSendMessage(QString msg);

public slots:
    /**
     * \brief Slot that checks if there's a world object. If so, initialize time variables
     * and start the simulation loop. Otherwise exit.
     * \return void
     */
    virtual void startSimulation();

    /**
     * \brief Slot that sets the simulating flag to false to stop the simulation loop.
     * \return void
     */
    virtual void stopSimulation();

    /**
     * \brief Slot that simulates a single time step. This function calls itself, unless
     * the flag "simulateOneFrame" is set to true.
     * \return void
     */
    virtual void simulateTimeStep();

    /**
     * \brief Slot that sets the "simulateOneFrame" to true and simulate a time step
     * \return void
     */
    virtual void simulateSingleTimeStep();

protected:
    /**
     * \brief Adds a GripTimeslice to the timeline vector in order to store the
     * world time and state in the timeline for playback or movie saving at a later time.
     * \param worldToAdd dart::simulation::World object of which to save the time and state.
     * \return void
     */
    void addWorldToTimeline(const dart::simulation::World& worldToAdd);

    /// World object received from creator that we need to simulate
    dart::simulation::World* _world;
    ViewerWidget* _viewer;

    /// Array of GripTimeSlice objects for simulation/kinematic playback
    std::vector<GripTimeslice>* _timeline;

    /// List of plugin pointers in order call their functions every timestep of simulation
    QList<GripTab*>* _plugins;

    /// Local thread to move object into
    QThread* _thread;

    double _simulationDuration;  ///< Simulation time in realtime
    double _simulationStartTime; ///< Initial system clock time when simulation started
    double _simTimeRelToRealTimeInstantaneous; ///< Simulation time relative to realtime (ie. 1.0 is realtime. 0.5 is half the speed of realtime)
    double _prevTime; ///< Real time on the last time step

    bool _simulating; ///< Bool for whether or not we are simulating
    bool _simulateOneFrame; ///< Bool for whether or not to simulate only one frame
    bool _debug; ///< Bool for whether or not to print debug output to standard error
};

#endif // GRIP_SIMULATION_H
