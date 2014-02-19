/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
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

#ifdef DEBUG_BUILD
#define DEBUG(x) do { std::cerr << x; } while (0); std::cerr << std::endl;
#else
#define DEBUG(x)
#endif
#include "DartNode.h"
#include "SkeletonNode.h"

using namespace dart;
using namespace osgDart;

DartNode::DartNode(){}

dynamics::Skeleton* DartNode::parseRobotUrdf(std::string urdfFile)
{
    // Load robot model from urdf and check if valid
    utils::DartLoader loader;
    dynamics::Skeleton* robot = loader.parseSkeleton(urdfFile);
    if(!robot) {
        std::cerr << "Error parsing robot urdf " << urdfFile << std::endl;
        return NULL;
    } else {
        DEBUG("Successfully parsed robot urdf " << urdfFile);
        return robot;
    }
}

simulation::World* DartNode::parseWorldUrdf(std::string urdfFile)
{
    // Load world model from urdf and check if valid
    utils::DartLoader loader;
    simulation::World* world = loader.parseWorld(urdfFile);
    if(!world) {
        std::cerr << "Error parsing world urdf " << urdfFile << std::endl;
        return NULL;
    } else {
        DEBUG("Successfully parsed world urdf " << urdfFile);
        return world;
    }
}


int DartNode::addWorld(std::string urdfFile)
{
    simulation::World* world = parseWorldUrdf(urdfFile);
    if(world) {
        addWorld(*world);
        return 1;
    } else {
        std::cerr << "Not adding world" << std::endl;
        return 0;
    }
}

int DartNode::addRobot(std::string urdfFile)
{
    dynamics::Skeleton* robot = parseRobotUrdf(urdfFile);
    if(robot) {
        addRobot(*robot);
        return 1;
    } else {
        return 0;
    }
}

size_t DartNode::addRobot(dynamics::Skeleton& robot)
{
    _robots.push_back(&robot);

    osgDart::SkeletonNode* skel = new osgDart::SkeletonNode(robot);
    this->addChild(skel);
    std::cerr << "Added robot:\n\t" << robot.getName() << std::endl;

    return _robots.size()-1;
}

dynamics::Skeleton* DartNode::getRobot(size_t robotIndex)
{
    if(robotIndexIsValid(robotIndex)) {
        return _robots[robotIndex];
    } else {
        return NULL;
    }
}

int DartNode::robotIndexIsValid(size_t robotIndex)
{
    if(robotIndex < _robots.size() && robotIndex >= 0) {
        return 1;
    } else {
        std::cerr << "Requested an invalid robot index of " << robotIndex
                  << " at line " << __LINE__ << " of " << __FILE__ << std::endl;
        return 0;
    }
}

void DartNode::printRobotInfo(size_t robotIndex)
{
    std::cout << "Robot[" << robotIndex << "]: "
              << "\n\tName: " << _robots[robotIndex]->getName()
              << "\n\tBodyNodes: " << _robots[robotIndex]->getNumBodyNodes()
              <<
    std::endl;
}

size_t DartNode::addWorld(simulation::World& world)
{
    DEBUG("Added world with the following objects:");
    for(int i=0; i<world.getNumSkeletons(); ++i) {
        _robots.push_back(world.getSkeleton(i));
        DEBUG("    " << world.getSkeleton(i)->getName());
        osgDart::SkeletonNode* skel = new osgDart::SkeletonNode(*world.getSkeleton(i));
        this->addChild(skel);
    }

    return _robots.size()-1;
}
