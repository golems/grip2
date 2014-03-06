

#include "GripSimulation.h"
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace dart;

GripSimulation::GripSimulation(bool debug) : _world(NULL), _debug(debug)
{

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
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        _world->step();

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
        std::cerr << time_span.count() << std::endl;
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

void GripSimulation::doBeforeSimulationTimeStep()
{

}

void GripSimulation::doAfterSimulationTimeStep()
{

}
