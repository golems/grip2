

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

GripSimulation::GripSimulation(MainWindow *gripWindow, bool debug) :
    _world(NULL), _debug(debug), _thread(new QThread), _gripWindow(gripWindow)
{
    this->moveToThread(_thread);
    _thread->start();
}


GripSimulation::~GripSimulation()
{

}

void GripSimulation::setWorld(simulation::World *world)
{
    _world = world;
    _world->checkCollision(false);
    if(_debug) {
        std::cerr << "World: "
                  << "\n\tGravity: " << _world->getGravity().transpose()
                  << "\n\tTimestep: " << _world->getTimeStep()
                  << "\n\tTime: " << _world->getTime()
                  << std::endl;
    }
}

void GripSimulation::startSimulation()
{
    if(_debug) {
        std::cerr << "Simulating" << std::endl;
    }

    _simulating = true;

    if(_world) {
        simulateTimeStep();
    } else {
        std::cerr << "Not simulating because there's no world yet" << std::endl;
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
        std::cerr << "World: "
                  << "\n\tSkeletons: " << _world->getNumSkeletons()
                  << "\n\tTime: " << _world->getTime()
                  << "\n\tStep: " << _world->getTimeStep()
                  << std::endl;
        _world->step();


        // Run each tabs doBeforeSimulationTimeStep function
        // for each tab
        //     tabs->doAfterSimulationTimeStep
        // end

    }
    QMetaObject::invokeMethod(this, "simulateTimeStep", Qt::QueuedConnection);

}



void GripSimulation::stopSimulation()
{
    if(_debug) {
        std::cerr << "Stoppin simulation" << std::endl;
    }
    _simulating = false;
}
