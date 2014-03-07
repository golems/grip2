

#include "GripSimulation.h"
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <iostream>

using namespace dart;

GripSimulation::GripSimulation()
{

}


GripSimulation::~GripSimulation()
{

}

void GripSimulation::setWorld(simulation::World *world)
{
    mWorld = world;
    std::cerr << "World: "
              << "\n\tGravity: " << mWorld->getGravity().transpose()
              << "\n\tTimestep: " << mWorld->getTimeStep()
              << "\n\tTime: " << mWorld->getTime()
              << std::endl;
}

void GripSimulation::startSimulation()
{
    std::cerr << "Simulating" << std::endl;
    mSimulating = true;
    size_t i = 0;
    while(mSimulating) {
//        std::cerr << "[Grip] Time: " << mWorld->getTime() << std::endl;
        mWorld->step();
//        if(mWorld->getSkeleton(0)->getJoint(0)->getLocalTransform().translation().z() <= 0) {
//            mSimulating = false;
//        }
        ++i;
    }
    std::cerr << "Stopping Simulation at time: " << mWorld->getTime() << std::endl;
}

void GripSimulation::stopSimulation()
{
    std::cerr << "Stoppin simulation" << std::endl;
    mSimulating = false;
}

void GripSimulation::doBeforeSimulationTimeStep()
{

}

void GripSimulation::doAfterSimulationTimeStep()
{

}
