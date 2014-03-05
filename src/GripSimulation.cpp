

#include "GripSimulation.h"
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <iostream>

using namespace dart;

GripSimulation::GripSimulation(bool debug) : _debug(debug)
{

}


GripSimulation::~GripSimulation()
{

}

void GripSimulation::setWorld(simulation::World *world)
{
    _world = world;
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

    while(_simulating) {
          _world->step();
    }

    if(_debug) {
        std::cerr << "Stopping Simulation at time: " << _world->getTime() << std::endl;
    }
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
