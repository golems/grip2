

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
    connect(this, SIGNAL(destroyed()), _thread, SLOT(quit()));
    connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
    connect(this, SIGNAL(simulationStoppedSignal()), parent, SLOT(simulationStopped()));
    this->moveToThread(_thread);
    _thread->start();
}


GripSimulation::~GripSimulation()
{
    _thread->deleteLater();
}

void GripSimulation::reset()
{
    _timeline.clear();
    _simulationDuration = 0;
    _simulationStartTime = 0;
    _simTimeRelToRealTimeInstantaneous = 0;
    _simTimeRelToRealTimeOverall = 0;
    _prevTime = 0;
}

void GripSimulation::addWorldToTimeline(const dart::simulation::World& worldToAdd)
{
    GripTimeslice timeslice;
    timeslice.time = worldToAdd.getTime();
    timeslice.state = worldToAdd.getState();
    _timeline.push_back(timeslice);
}

void GripSimulation::startSimulation()
{
    if(_debug) {
        std::cerr << "Simulating" << std::endl;
    }

    _simulating = true;

    if(_world) {
        _simulationStartTime = grip::getTime();
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
        _simTimeRelToRealTimeOverall = _world->getTime() / _simulationDuration;
        _prevTime = curTime;


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
            std::cerr << "Emitting stop signal and exiting simulation loop" << std::endl;
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
        std::cerr << "Simulating a single timestep" << std::endl;
        this->simulateTimeStep();
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
