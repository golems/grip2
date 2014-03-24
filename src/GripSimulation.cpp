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

// Local includes
#include "GripSimulation.h"
#include "gripTime.h"

// DART includes
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>

// Standard C++ includes
#include <iostream>
#include <iomanip>

// QT includes
#include <QThread>

GripSimulation::GripSimulation(dart::simulation::World* world, MainWindow* parent, bool debug) :
    QObject(),
    _world(world),
    _debug(debug),
    _thread(new QThread),
    _simulating(false),
    _simulateOneFrame(false)
{
    // Signals and slots for the worker object and thread
    connect(this, SIGNAL(destroyed()), _thread, SLOT(quit()));
    connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));

    // Signal and slot for informing the parent that the simulation loop in stopped
    connect(this, SIGNAL(simulationStoppedSignal()), parent, SLOT(simulationStopped()));

    // Signal and slot for sending the simulation time relative to real time (instantaneous) to the parent
    connect(this, SIGNAL(signalRelTimeChanged(double)), parent, SLOT(setSimulationRelativeTime(double)));

    // Signal and slot for sending a message to the status bar
    connect(this, SIGNAL(setMessage(QString)), parent, SLOT(setMessageSlot(QString)));

//    connect(this, SIGNAL(signalAddTimesliceToTimeline(GripTimeslice)), parent, SLOT(GripMainWindow::slotAddTimesliceToTimeline(const GripTimeslice&)));

    this->moveToThread(_thread);
    _thread->start();
}


GripSimulation::~GripSimulation()
{
    _thread->deleteLater();
}

void GripSimulation::reset()
{
    _simulationDuration = 0;
    _simulationStartTime = 0;
    _simTimeRelToRealTimeInstantaneous = 0;
    _prevTime = 0;
}

void GripSimulation::addWorldToTimeline(const dart::simulation::World& worldToAdd)
{
    GripTimeslice timeslice;
    timeslice.time = worldToAdd.getTime();
    timeslice.state = worldToAdd.getState();
    emit signalAddTimesliceToTimeline(timeslice);
}

void GripSimulation::startSimulation()
{
    if(_debug) {
        std::cerr << "Simulating" << std::endl;
    }

    _simulating = true;

    if(_world) {
        emit this->setMessage(tr("Simulating"));
        _simulationStartTime = grip::getTime();
        _prevTime = grip::getTime();

        simulateTimeStep();
    } else {
        emit setMessage(tr("Not simulating b/c there's no world"));
        std::cerr << "[GripSimulation] Not simulating because there's no world yet. From line "
                  << __LINE__ << " of " << __FILE__
                  << std::endl;
    }

}
void GripSimulation::simulateTimeStep()
{
    if(_simulating) {

        // Run each tabs doBeforeSimulationTimeStep function
        // for each tab
        //     tabs->doBeforeSimulationTimeStep
        // end

        // Simulate timestep by stepping the world dynamics forward one step
        _world->step();
        addWorldToTimeline(*_world);

        // Run each tabs doBeforeSimulationTimeStep function
        // for each tab
        //     tabs->doAfterSimulationTimeStep
        // end

        double curTime = grip::getTime();
        double timeStepDuration = curTime - _prevTime;
        _simulationDuration = _simulationDuration + timeStepDuration;
        _simTimeRelToRealTimeInstantaneous = _world->getTimeStep() / timeStepDuration;
        _prevTime = curTime;
        emit signalRelTimeChanged(_simTimeRelToRealTimeInstantaneous);

//        std::cerr << "Sim2 | Real | RelInst | RelOverall: "
//                  << _world->getTime() << " | "
//                  << _simulationDuration << " | "
//                  << _simTimeRelToRealTimeInstantaneous << " | "
//                  << _simTimeRelToRealTimeOverall
//                  << std::endl;

        if(_simulateOneFrame) {
            return;
        } else {
            // Call this method again using invokeMethod to give the EventLoop
            // time to check for other signals
            QMetaObject::invokeMethod(this, "simulateTimeStep", Qt::QueuedConnection);
        }
    } else { // Get out of this function so we don't call ourselves again
        if(_debug) {
            std::cerr << "[GripSimulation] Emitting stop signal and exiting simulation loop" << std::endl;
        }
        emit simulationStoppedSignal();
        return;
    }


}

void GripSimulation::simulateSingleTimeStep()
{
    _simulating = true;
    _simulateOneFrame = true;

    if(_debug) {
        std::cerr << "[GripSimulation] Simulating a single timestep" << std::endl;
        this->simulateTimeStep();
    }

    _simulating = false;
    _simulateOneFrame = false;
}

void GripSimulation::stopSimulation()
{
    if(_debug) {
        std::cerr << "[GripSimulation] Stoppin simulation" << std::endl;
    }
    emit setMessage(tr("Simulation Stopped"));
    _simulating = false;
}
