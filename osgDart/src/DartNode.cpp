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
#include <dart/collision/CollisionDetector.h>
#include <dart/constraint/Constraint.h>
#include <dart/constraint/ConstraintDynamics.h>

// osgDart includes
#include "DartNode.h"
#include "SkeletonNode.h"
#include "DartNodeCallback.h"
#include "osgUtils.h"
#include "WorldVisuals.h"

// Standard includes
#include <stdexcept>

using namespace osgDart;

DartNode::DartNode(bool debug)
    : _world(0),
      _debug(debug),
      _showContactForces(0)
{
    this->setName("DartNode");
    this->setUpdateCallback(new DartNodeCallback);
}

DartNode::~DartNode()
{

}

void DartNode::update()
{
    SkeletonNodeMap::const_iterator it;
    for (int i=0; i<_world->getNumSkeletons(); ++i) {
        it = _skelNodeMap.find(_world->getSkeleton(i));
        if (it != _skelNodeMap.end()) {
            _skelNodeMap.at(_world->getSkeleton(i))->update();
        } else {
            _skeletons.push_back(_world->getSkeleton(i));
            osgDart::SkeletonNode* skelNode = new osgDart::SkeletonNode(*_world->getSkeleton(i), _debug);
            _skeletonNodes.push_back(skelNode);
            _skelNodeMap.insert(std::make_pair(_world->getSkeleton(i), skelNode));
            this->addChild(skelNode);
        }
    }

    // Update contact forces
    if (_showContactForces) {
        _updateContactForces();
    }
}

void DartNode::_updateContactForces()
{
    // FIXME this should be updated based on the selected node in the Qt treeview
//    dart::dynamics::BodyNode* selectedNode = new dart::dynamics::BodyNode;

    // If we have a world and contraint handler, get all the contact forces and create OpenSceneGraph
    // vector to represent them
    if (_world && _world->getConstraintHandler()) {
        size_t numContacts = _world->getConstraintHandler()->getCollisionDetector()->getNumContacts();
        std::vector<Eigen::Vector3d> contactPoints(numContacts);
        std::vector<Eigen::Vector3d> contactForces(numContacts);
        float forceVectorLengths[numContacts];
//        bool nodeIsSelected[numContacts];
        float maxForceVectorLength = 0;

        // Extract contact force from world
        for (size_t i = 0; i < numContacts; ++i) {
            dart::collision::Contact contact = _world->getConstraintHandler()->getCollisionDetector()->getContact(i);

            contactPoints[i] = contact.point;
            contactForces[i] = contact.force;
            forceVectorLengths[i] = contactForces[i].norm();
            if (forceVectorLengths[i] != forceVectorLengths[i]) {
                forceVectorLengths[i] = 0;
            }

            // Update max force vector length variable if current force vector length is larger than max
            if (forceVectorLengths[i] > maxForceVectorLength) {
                maxForceVectorLength = forceVectorLengths[i];
            }
//            nodeIsSelected[i] = false;
//            // If either of the BodyNodes in contact are the user-selected node, mark it
//            if (contact.collisionNode1->getBodyNode() == selectedNode
//                    || contact.collisionNode2->getBodyNode() == selectedNode) {
//                nodeIsSelected[i] = true;
//            }
//            if (contact.collisionNode1->getBodyNode()->getName() == "ground"
//                    && contact.collisionNode2->getBodyNode()->getName() == "link_0") {
//                std::cerr << "Force[" << i << "]: " << forceVectorLengths[i] /*contactForces[i].transpose()*/ << std::endl;
//            }
        }

        // Create force arrows to render
        float maxLength = 0.3; // Desired max length of force vectors (meters)

        for (size_t i = 0; i < numContacts; ++i) {
            if (forceVectorLengths[i] > 0.01) {
                osg::ref_ptr<ContactForceVisual> contactForceLine;
                // If we have some, use existing contactForceArrows and update them to the
                // current contact force values
                if (_contactForceArrows.size() > i) {
                    contactForceLine = _contactForceArrows[i];
                    contactForceLine->update(forceVectorLengths[i] / maxForceVectorLength * maxLength, contactPoints[i], -contactForces[i]);
                // Otherwise create a new one and add it to the existing ones.
                // And then add it as child to the DartNode. Only add if we're creating new one
                } else {
                    contactForceLine = new ContactForceVisual(_debug);
                    contactForceLine->createForceVector(forceVectorLengths[i] / maxForceVectorLength * maxLength, contactPoints[i], -contactForces[i]);
                    _contactForceArrows.push_back(contactForceLine);
                    this->addChild(contactForceLine);
                }
                // Show the node
                contactForceLine->setNodeMask(0xffffffff);
            }
        }

        // Hide unused contact force arrows
        if (numContacts > 0) {
            for (size_t i = numContacts; i < _contactForceArrows.size(); ++i) {
                _contactForceArrows[i]->setNodeMask(0x0);
            }
        }
    } else {
//        delete selectedNode;
        return;
    }
//    delete selectedNode;
}

void DartNode::setContactForcesVisible(bool makeVisible)
{
    if(_debug) {
        std::cerr << "[DartNode] " << (makeVisible ? "Showing " : "Hiding ") << "contact forces" << std::endl;
    }

    // If there's a world and constraint handler, show/hide contact force arrows from 0 to number of contact forces
    // and hide the remaining unused ones
    if (_world && _world->getConstraintHandler()) {
        size_t numContacts = _world->getConstraintHandler()->getCollisionDetector()->getNumContacts();
        if(_debug) {
            std::cerr << "Number of contact forces: " << numContacts << std::endl;
        }
        for(size_t i = 0; i < numContacts && i < _contactForceArrows.size(); ++i) {
            _contactForceArrows[i]->setNodeMask(makeVisible ? 0xffffffff : 0x0);
        }
        for(size_t i = numContacts; i < _contactForceArrows.size(); ++i) {
            _contactForceArrows[i]->setNodeMask(0x0);
        }
    }
    _showContactForces = makeVisible;
}

void DartNode::setJointAxesVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting Joint axis visibility for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (makeVisible ? "True" : "False") << std::endl;
    }
    for (size_t i=0; i<_skeletonNodes.size(); ++i) {
        _skeletonNodes[i]->setJointAxesVisible(makeVisible);
    }
}

void DartNode::setBodyNodeAxesVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting BodyNode axes visibility for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (makeVisible ? "True" : "False") << std::endl;
    }
    for (size_t i=0; i<_skeletonNodes.size(); ++i) {
        _skeletonNodes[i]->setBodyNodeAxesVisible(makeVisible);
    }
}

void DartNode::setSkeletonCoMVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting Skeleton CoM visibility for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (makeVisible ? "True" : "False") << std::endl;
    }
    for (size_t i=0; i<_skeletonNodes.size(); ++i) {
        _skeletonNodes[i]->setSkeletonCoMVisible(makeVisible);
    }
}

void DartNode::setSkeletonCoMProjectedVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting Skeleton projected CoM visibility for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (makeVisible ? "True" : "False") << std::endl;
    }
    for(size_t i=0; i<_skeletonNodes.size(); ++i) {
        _skeletonNodes[i]->setSkeletonCoMProjectedVisible(makeVisible);
    }
}

void DartNode::setSkeletonCollisionMeshOn(bool enable)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting Skeleton render collision mesh for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (enable ? "True" : "False") << std::endl;
    }
    for (size_t i=0; i<_skeletonNodes.size(); ++i) {
        if (enable) {
            _skeletonNodes[i]->setSkeletonRenderMode(osgDart::RENDER_COLLISION_MESH);
        } else {
            _skeletonNodes[i]->setSkeletonRenderMode(osgDart::RENDER_VISUAL_MESH);
        }
    }
}

void DartNode::setSkeletonWireFrameOn(bool enable)
{
    if (_debug) {
        std::cerr << "[DartNode] Setting Skeleton render wireframe mode for "
                  << _skeletonNodes.size() << " skeletons to "
                  << (enable ? "True" : "False") << std::endl;
    }
    for (size_t i=0; i<_skeletonNodes.size(); ++i) {
        if (enable) {
            _skeletonNodes[i]->setSkeletonRenderMode(osgDart::RENDER_WIREFRAME_ON);
        } else {
            _skeletonNodes[i]->setSkeletonRenderMode(osgDart::RENDER_WIREFRAME_OFF);
        }
    }
}

dart::dynamics::Skeleton* DartNode::parseSkeletonUrdf(std::string urdfFile)
{
    // Load robot model from urdf and check if valid
    dart::utils::DartLoader loader;
    dart::dynamics::Skeleton* skeleton = loader.parseSkeleton(urdfFile);
    if (!skeleton && _debug) {
        std::cerr << "[DartNode] Error parsing robot urdf " << urdfFile  << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if (_debug) {
            std::cerr << "[DartNode] Successfully parsed robot urdf " << urdfFile << std::endl;
        }

        return skeleton;
    }
}

dart::simulation::World* DartNode::parseWorldSdf(std::string sdfFile)
{
    dart::utils::SdfParser loader;
    dart::simulation::World* world = loader.readSdfFile(sdfFile);
    if (!world && _debug) {
        std::cerr << "[[DartNode] Error parsing world sdf " << sdfFile << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if (_debug) {
            std::cerr << "[DartNode] Successfully parsed world sdf " << sdfFile << std::endl;
        }
        return world;
    }
}

dart::simulation::World* DartNode::parseWorldUrdf(std::string urdfFile)
{
    // Load world model from urdf and check if valid
    dart::utils::DartLoader loader;
    dart::simulation::World* world = loader.parseWorld(urdfFile);
    if (!world && _debug) {
        std::cerr << "[DartNode] Error parsing world urdf " << urdfFile << " on line " << __LINE__ << " of " << __FILE__ << std::endl;
        return NULL;
    } else {
        if (_debug) {
            std::cerr << "[DartNode] Successfully parsed world urdf " << urdfFile << std::endl;
        }
        return world;
    }
}

size_t DartNode::addWorld(std::string file)
{
    std::string extension = file.substr(file.find_last_of(".") + 1);
    if (extension == "urdf") {
        dart::simulation::World* world = parseWorldUrdf(file);
        if (world) {
            this->addWorld(world);
        } else {
            dart::dynamics::Skeleton* skel = parseSkeletonUrdf(file);
            if (skel) {
                this->addSkeleton(*skel);
            }
        }
    } else if (extension == "sdf") {
        dart::simulation::World* world = parseWorldSdf(file);
        if (world) {
            this->addWorld(world);
        }
    } else {
        dart::simulation::World* world = parseWorldSdf(file);
        if (world) {
            this->addWorld(world);
        } else {
            world = parseWorldUrdf(file);
            if (world) {
                this->addWorld(world);
            }
        }
    }
    if (!_world->getNumSkeletons()) {
        std::cerr << "[DartNode] Error parsing scene file. Line "
                  << __LINE__ << " of " << __FILE__ << std::endl;
    }
    return _skeletons.size();
}

size_t DartNode::addWorldFromSdf(std::string sdfFile)
{
    dart::simulation::World* world = parseWorldSdf(sdfFile);
    if (world) {
        addWorld(world);
    } else {
        delete world;
        std::cerr << "[DartNode] Not adding world on line " << __LINE__ << " of " << __FILE__ << std::endl;
    }
    return _skeletons.size();
}

size_t DartNode::addSkeleton(std::string urdfFile)
{
    dart::dynamics::Skeleton* skeleton = parseSkeletonUrdf(urdfFile);
    if (skeleton) {
        addSkeleton(*skeleton);
        return _skeletons.size()-1;
    } else {
        return _skeletons.size()-1;
    }
}

size_t DartNode::addSkeleton(dart::dynamics::Skeleton& skeleton)
{
    if (!_world) {
        if (_debug) {
            std::cerr << "[DartNode] Creating world in DartNode" << std::endl;
        }
        _world = new dart::simulation::World();
    }
    _world->addSkeleton(&skeleton);
    _skeletons.push_back(&skeleton);

    osg::ref_ptr<osgDart::SkeletonNode> skelNode = new osgDart::SkeletonNode(skeleton, _debug);
    _skeletonNodes.push_back(skelNode);
    _skelNodeMap.insert(std::make_pair(&skeleton, skelNode));
    this->addChild(skelNode);
    if (_debug) {
        std::cerr << "[DartNode] Added robot:\n\t" << skeleton.getName() << std::endl;
    }

    return _skeletons.size()-1;
}

dart::dynamics::Skeleton* DartNode::getSkeleton(size_t skeletonIndex)
{
    if (skeletonIndexIsValid(skeletonIndex)) {
        return _world->getSkeleton(skeletonIndex);;
    } else {
        if (_world) {
            std::cerr << "[DartNode] Error. There are only " << _world->getNumSkeletons() << " robots in this DartNode"
                      << std::endl;
        }
        return NULL;
    }
}

int DartNode::removeSkeleton(const dart::dynamics::Skeleton* skeletonToRemove)
{
    try {
        this->removeChild(_skelNodeMap.at(skeletonToRemove));
    } catch (const std::out_of_range& oor) {
        std::cerr << "[DartNode] Error: Out-of-range " << oor.what() << std::endl;
        return 0;
    }

    if (_skelNodeMap.erase(skeletonToRemove)) {
        return 1;
    } else {
        std::cerr << "[DartNode] Tried to remove a robot that doesn't exist" << std::endl;
        return 0;
    }
}

int DartNode::removeSkeleton(size_t skeletonIndex)
{
    if (skeletonIndexIsValid(skeletonIndex)) {
        std::cerr << "[DartNode] Removing skeleton named: " << _skeletons.at(skeletonIndex)->getName() << std::endl;
        if (removeSkeleton(_skeletons.at(skeletonIndex))) {
            _skeletons.erase(_skeletons.begin() + skeletonIndex-1);
            return 1;
        } else {
            std::cerr << "[DartNode] Couldn't remove skeleton" << std::endl;
            return 0;
        }
    } else {
        return 0;
    }
}

void DartNode::reset()
{
//    if (this->getNumChildren()) {
    // Remove all the children from the DartNode
    this->removeChildren(0, this->getNumChildren());
    for (size_t i = 0; i < _contactForceArrows.size(); ++i) {
        std::cerr << "Pre Ref count forces: " << _contactForceArrows.at(i)->referenceCount() << std::endl;
    }

    for (size_t i = 0; i < _skeletonNodes.size(); ++i) {
        std::cerr << "Ref count skelnode: " << _skeletonNodes.at(i)->referenceCount() << std::endl;
    }

    for (size_t i = 0; i < _contactForceArrows.size(); ++i) {
        std::cerr << "Ref count: " << _contactForceArrows.at(i)->referenceCount() << std::endl;
                _contactForceArrows.at(i)->unref();
        std::cerr << "Ref count now: " << _contactForceArrows.at(i)->referenceCount() << std::endl;
    }

    for (size_t i = 0; i < _skeletonNodes.size(); ++i) {
        std::cerr << "Ref count node: " << _skeletonNodes.at(i)->referenceCount() << std::endl;
        std::cerr << "Skel Parents: " << _skeletonNodes[i]->getNumParents() << std::endl;
                _skeletonNodes.at(i)->unref();
        std::cerr << "Ref count now node: " << _skeletonNodes.at(i)->referenceCount() << std::endl;
    }

    if (_skeletonNodes.at(0)) {
        std::cerr << "Still valid" << std::endl;
    } else {
        std::cerr << "NULL  Pointer" << std::endl;
    }

    _contactForceArrows.clear();
    _skeletonNodes.clear();
    _skelNodeMap.clear();
    _skeletons.clear();

    std::cerr << "SkeletonNodes: " << _skeletonNodes.size() << std::endl;
    std::cerr << "Skeletons: " << _skeletons.size() << std::endl;
    std::cerr << "SkelMaps: " << _skelNodeMap.size() << std::endl;
    std::cerr << "Forces: " << _contactForceArrows.size() << std::endl;
//    }
    assert(this->getNumChildren() == 0);
}

void DartNode::hideSkeleton(int i)
{
    if (skeletonIndexIsValid(i)) {
//        _skeletonNodes(i)->setNodeMask(0x0);
    }
}

dart::simulation::World* DartNode::getWorld()
{
    if (_world) {
        return _world;
    } else {
        if (_debug) {
            std::cerr << "[DartNode] No world exists in the DartNode yet" << std::endl;
        }
        return NULL;
    }
}

int DartNode::skeletonIndexIsValid(size_t skeletonIndex)
{
    if (skeletonIndex < _skeletons.size() && skeletonIndex >= 0) {
        return 1;
    } else {
        std::cerr << "[DartNode] Requested an invalid robot index of " << skeletonIndex
                  << " at line " << __LINE__ << " of " << __FILE__ << std::endl;
        return 0;
    }
}

void DartNode::printInfo()
{
    std::cout << "DartNode Robots:";
    for (size_t i=0; i<_skeletons.size(); ++i) {
        std::cout << "\n    " << _skeletons[i]->getName()
                  << ": " << _skeletons[i]->getNumBodyNodes() << " BodyNodes";
    }
    std::cout << "\n# SkeletonNode: " << _skeletonNodes.size() << std::endl;
}


void DartNode::setSkeletonTransparency(const dart::dynamics::Skeleton& skel, float transparencyValue)
{
    if (_world && _world->getSkeleton(skel.getName())) {
        osgGolems::setTransparency(_skelNodeMap.at(&skel), transparencyValue);
    } else {
        std::cerr << "[DartNode] Error setting Skeleton transparency" << std::endl;
    }
}

void DartNode::setBodyNodeTransparency(const dart::dynamics::BodyNode& node, float transparencyValue)
{
    if (_world && _world->getSkeleton(node.getSkeleton()->getName())) {
        _skelNodeMap.at(node.getSkeleton())->setBodyNodeTransparency(node, transparencyValue);
    } else {
        std::cerr << "[DartNode] Error setting BodyNode transparency" << std::endl;
    }
}

size_t DartNode::getNumSkeletons()
{
    return _skeletons.size();
}

size_t DartNode::addWorld(dart::simulation::World* world)
{
    if (!_world) {
        _world = world;
    } else {
        if (_debug) {
            std::cerr << "[DartNode] This DartNode already has a world object. Not going to create another one" << std::endl;
        }
        for (int i=0; i<world->getNumSkeletons(); ++i) {
            _world->addSkeleton(world->getSkeleton(i));
        }
    }

    if (_debug) {
        std::cerr << "[DartNode] Added world with the following " << world->getNumSkeletons() << " objects:";
    }
    for (int i=0; i<world->getNumSkeletons(); ++i) {
        _skeletons.push_back(world->getSkeleton(i));
        if (_debug) {
            std::cerr << "    " << world->getSkeleton(i)->getName() << std::endl;
        }
        osgDart::SkeletonNode* skelNode = new osgDart::SkeletonNode(*world->getSkeleton(i), _debug);
        _skeletonNodes.push_back(skelNode);
        _skelNodeMap.insert(std::make_pair(world->getSkeleton(i), skelNode));
        this->addChild(skelNode);
    }

    return _skeletons.size()-1;
}


