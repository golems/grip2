

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

using namespace dart;

GripSimulation::GripSimulation(bool debug) :
    _world(NULL),
    _debug(debug),
    _thread(new QThread),
    _simulating(false),
    _simulateOneFrame(false)
{
    this->moveToThread(_thread);
    _thread->start();
}


GripSimulation::~GripSimulation()
{
    _thread->quit();
    _thread->wait();
}

void GripSimulation::setWorld(simulation::World *world)
{
    // Assign world object and add initial state to the timeline
    _world = world;
    addWorldToTimeline(*_world);

    if(_debug) {
        std::cerr << "World: "
                  << "\n\tGravity: " << _world->getGravity().transpose()
                  << "\n\tTimestep: " << _world->getTimeStep()
                  << "\n\tTime: " << _world->getTime()
                  << std::endl;
    }
}

void GripSimulation::addWorldToTimeline(const dart::simulation::World& worldToAdd)
{
    timeslice slice;
    slice.time = worldToAdd.getTime();
    slice.state = worldToAdd.getState();
    _timeline.push_back(slice);
}

void GripSimulation::startSimulation()
{
    if(_debug) {
        std::cerr << "Simulating" << std::endl;
    }

    _simulating = true;

    if(_world) {
        if(_world->getTime() == 0) {
            _simulationStartTime = grip::getTime();
        }
        _prevTime = grip::getTime();

        simulateTimeStep();
    } else {
        std::cerr << "Not simulating because there's no world yet. From line "
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

        double t1 = grip::getTime();

        // Simulate timestep by stepping the world dynamics forward one step
        _world->step();
        addWorldToTimeline(*_world);

        std::cerr << grip::getTime() - t1 << std::endl;

        // Run each tabs doBeforeSimulationTimeStep function
        // for each tab
        //     tabs->doAfterSimulationTimeStep
        // end

        double curTime = grip::getTime();
        double timeStepDuration = curTime - _prevTime;
//        simulationTime = ;
        _simTimeRelToRealTime = timeStepDuration / _world->getTimeStep();

        if(_simulateOneFrame) {
            return;
        } else {
            // Call this method again using invokeMethod to give the EventLoop
            // time to check for other signals
            QMetaObject::invokeMethod(this, "simulateTimeStep", Qt::QueuedConnection);
        }
    } else { // Get out of this function so we don't call ourselves again
        return;
    }


}

void GripSimulation::simulateSingleTimeStep()
{
    _simulating = true;
    _simulateOneFrame = true;

    if(_debug) {
        std::cerr << "Simulating a single timestep" << std::endl;
        emit this->simulateTimeStep();
    }

    _simulating = false;
    _simulateOneFrame = false;
}

void GripSimulation::stopSimulation()
{
    if(_debug) {
        std::cerr << "Stoppin simulation" << std::endl;
    }
    _simulating = false;
}
