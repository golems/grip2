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

// DART includes
#include <dart/utils/urdf/DartLoader.h>
#include <dart/utils/sdf/SdfParser.h>

// osgDart includes
#include "DartNode.h"
#include "SkeletonNode.h"
#include "DartNodeCallback.h"
#include "osgUtils.h"

// Standard includes
#include <stdexcept>

using namespace dart;
using namespace osgDart;

DartNode::DartNode(bool debug) : _world(0), _debug(debug)
{
    this->setUpdateCallback(new DartNodeCallback);
}

void DartNode::update()
{
    for(size_t i=0; i<_skeletonNodes.size(); ++i) {
        _skeletonNodes[i]->update();
    }
}

dynamics::Skeleton* DartNode::parseSkeletonUrdf(std::string urdfFile)
{
    // Load robot model from urdf and check if valid
    utils::DartLoader loader;
    dynamics::Skeleton* skeleton = loader.parseSkeleton(urdfFile);
    if(!skeleton) {
        std::cerr << "[parseRobotUrdf] Error parsing robot urdf " << urdfFile  << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if(_debug) {
            std::cerr << "Successfully parsed robot urdf " << urdfFile << std::endl;
        }

        return skeleton;
    }
}

simulation::World* DartNode::parseWorldSdf(std::string sdfFile)
{
    utils::SdfParser loader;
    simulation::World* world = loader.readSdfFile(sdfFile);
    if(!world) {
        std::cerr << "[parseRobotSdf] Error parsing world sdf " << sdfFile << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if(_debug) {
            std::cerr << "Successfully parsed world sdf " << sdfFile << std::endl;
        }
        return world;
    }
}

simulation::World* DartNode::parseWorldUrdf(std::string urdfFile)
{
    // Load world model from urdf and check if valid
    utils::DartLoader loader;
    simulation::World* world = loader.parseWorld(urdfFile);
    if(!world) {
        std::cerr << "[parseWorldUrdf] Error parsing world urdf " << urdfFile << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if(_debug) {
            std::cerr << "Successfully parsed world urdf " << urdfFile << std::endl;
        }
        return world;
    }
}


size_t DartNode::addWorld(std::string file)
{
    // try urdf first
    simulation::World* world = parseWorldUrdf(file);
    if(world) {
        this->addWorld(world);
        return _skeletons.size();
    } else {
        dynamics::Skeleton* skel = parseSkeletonUrdf(file);
        if(skel) {
            this->addSkeleton(skel);
            return _skeletons.size();
        } else {
            world = parseWorldSdf(file);
            if(world) {
                this->addWorld(world);
                return _skeletons.size();
            } else {
                std::cerr << "[addWorld] Not adding world on line " << __LINE__ << " of " << __FILE__ << std::endl;
                return _skeletons.size();
            }
        }
    }
}

size_t DartNode::addWorldFromSdf(std::string sdfFile)
{
    simulation::World* world = parseWorldSdf(sdfFile);
    if(world) {
        addWorld(world);
        return _skeletons.size();
    } else {
        std::cerr << "[addWorldFromSdf] Not adding world on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return _skeletons.size();
    }
}

size_t DartNode::addSkeleton(std::string urdfFile)
{
    dynamics::Skeleton* skeleton = parseSkeletonUrdf(urdfFile);
    if(skeleton) {
        addSkeleton(skeleton);
        return _skeletons.size()-1;
    } else {
        return _skeletons.size()-1;
    }
}

size_t DartNode::addSkeleton(dynamics::Skeleton* skeleton)
{
    if(!_world) {
        if(_debug) {
            std::cerr << "Creating world in DartNode" << std::endl;
        }
        _world = new simulation::World();
    }
    _world->addSkeleton(skeleton);
    _skeletons.push_back(skeleton);

    osg::ref_ptr<osgDart::SkeletonNode> skelNode = new osgDart::SkeletonNode(skeleton, _debug);
    _skeletonNodes.push_back(skelNode);
    _skelNodeMap.insert(std::make_pair(skeleton, skelNode));
    this->addChild(skelNode);
    if(_debug) {
        std::cerr << "Added robot:\n\t" << skeleton->getName() << std::endl;
    }

    return _skeletons.size()-1;
}

dynamics::Skeleton* DartNode::getSkeleton(size_t skeletonIndex)
{
    if(skeletonIndexIsValid(skeletonIndex)) {
        return _world->getSkeleton(skeletonIndex);;
    } else {
        if(_world) {
            std::cerr << " There are only " << _world->getNumSkeletons() << " robots in this DartNode"
                      << std::endl;
        }
        return NULL;
    }
}

int DartNode::removeSkeleton(dart::dynamics::Skeleton* skeletonToRemove)
{
    try {
        this->removeChild(_skelNodeMap.at(skeletonToRemove));
    } catch(const std::out_of_range& oor) {
        std::cerr << "[removeRobot] Error: Out-of-range " << oor.what() << std::endl;
        return 0;
    }

    if(_skelNodeMap.erase(skeletonToRemove)) {
        return 1;
    } else {
        std::cerr << "[removeTried to remove a robot that doesn't exist" << std::endl;
        return 0;
    }
}

int DartNode::removeSkeleton(size_t skeletonIndex)
{
    if(skeletonIndexIsValid(skeletonIndex)) {
        std::cerr << "Removing skeleton named: " << _skeletons.at(skeletonIndex)->getName() << std::endl;
        if(removeSkeleton(_skeletons.at(skeletonIndex))) {
            _skeletons.erase(_skeletons.begin() + skeletonIndex-1);
            return 1;
        } else {
            std::cerr << "Couldn't remove skeleton" << std::endl;
            return 0;
        }
    } else {
        return 0;
    }
}

void DartNode::removeAllSkeletons()
{
    if(this->getNumChildren()) {
        this->removeChildren(0, this->getNumChildren());
        _skeletons.clear();
        _skeletonNodes.clear();
        _skelNodeMap.clear();
        _world = 0;
    }
}

void DartNode::hideSkeleton(int i)
{
    if(i < _skeletons.size()) {
        this->setValue(i, false);
    }
}

simulation::World* DartNode::getWorld()
{
    if(_world) {
        return _world;
    } else {
        if(_debug) {
            std::cerr << "No world exists in the DartNode yet" << std::endl;
        }
        return NULL;
    }
}

int DartNode::skeletonIndexIsValid(size_t skeletonIndex)
{
    if(skeletonIndex < _skeletons.size() && skeletonIndex >= 0) {
        return 1;
    } else {
        std::cerr << "Requested an invalid robot index of " << skeletonIndex
                  << " at line " << __LINE__ << " of " << __FILE__ << std::endl;
        return 0;
    }
}

void DartNode::printInfo()
{
    std::cout << "DartNode Robots:";
    for(int i=0; i<_skeletons.size(); ++i) {
        std::cout << "\n    " << _skeletons[i]->getName()
                  << ": " << _skeletons[i]->getNumBodyNodes() << " BodyNodes";
    }
    std::cout << std::endl;
}

size_t DartNode::getNumSkeletons()
{
    return _skeletons.size();
}

size_t DartNode::addWorld(simulation::World* world)
{
    if(!_world) {
        _world = world;
    } else {
        if(_debug) {
            std::cerr << "This DartNode already has a world object. Not going to create another one" << std::endl;
        }
        for(int i=0; i<world->getNumSkeletons(); ++i) {
            _world->addSkeleton(world->getSkeleton(i));
        }
    }

    if(_debug) {
        std::cerr << "Added world with the following " << world->getNumSkeletons() << " objects:";
    }
    for(int i=0; i<world->getNumSkeletons(); ++i) {
        _skeletons.push_back(world->getSkeleton(i));
        if(_debug) {
            std::cerr << "    " << world->getSkeleton(i)->getName() << std::endl;
        }
        osgDart::SkeletonNode* skelNode = new osgDart::SkeletonNode(world->getSkeleton(i), _debug);
        _skeletonNodes.push_back(skelNode);
        _skelNodeMap.insert(std::make_pair(world->getSkeleton(i), skelNode));
        this->addChild(skelNode);
    }

    return _skeletons.size()-1;
}


