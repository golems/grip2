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

// Local includes
#include "SkeletonNode.h"
#include "osgUtils.h"
#include "osgDartShapes.h"

// DART includes
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/EllipsoidShape.h>
#include <dart/dynamics/CylinderShape.h>
#include <dart/dynamics/MeshShape.h>
#include <dart/dynamics/RevoluteJoint.h>

// OpenSceneGraph includes
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/BlendFunc>

using namespace osgDart;

SkeletonNode::SkeletonNode(const dart::dynamics::Skeleton &skeleton, bool debug) :
    _rootBodyNode(*skeleton.getRootBodyNode()),
    _skeletonVisuals(new osgDart::SkeletonVisuals),
    _debug(debug)
{
    this->setName(_rootBodyNode.getSkeleton()->getName());
    _createSkeleton();

    osg::Material* mat = (osg::Material*)_bodyNodeGroupMap.at(&_rootBodyNode)->
            getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL);
    this->getOrCreateStateSet()->setAttribute(mat);
    this->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc);
}

SkeletonNode::~SkeletonNode()
{

}

void SkeletonNode::setJointAxesVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[SkeletonNode] " << (makeVisible ? "Showing " : "Hiding ")
                  << "Joint Axes for " << this->getName() << std::endl;
    }
    for (size_t i=0; i<_bodyNodeVisuals.size(); ++i) {
        if (_bodyNodeVisuals.at(i)->getJointAxisTF()) {
            _bodyNodeVisuals.at(i)->getJointAxisTF()->setNodeMask(makeVisible ? 0xffffffff : 0x0);
        }
    }
}

void SkeletonNode::setBodyNodeAxesVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[SkeletonNode] " << (makeVisible ? "Showing " : "Hiding ")
                  << "BodyNode Axes for " << this->getName() << std::endl;
    }
    for (size_t i=0; i<_bodyNodeVisuals.size(); ++i) {
        if (_bodyNodeVisuals.at(i)->getBodyNodeAxesTF()) {
            _bodyNodeVisuals.at(i)->getBodyNodeAxesTF()->setNodeMask(makeVisible ? 0xffffffff : 0x0);
        }
    }
}

void SkeletonNode::setSkeletonCoMVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[SkeletonNode] " << (makeVisible ? "Showing " : "Hiding ")
                  << "CoM for " << this->getName() << std::endl;
    }

    if (_skeletonVisuals->getCenterOfMassTF()) {
        _skeletonVisuals->getCenterOfMassTF()->setNodeMask(makeVisible ? 0xffffffff : 0x0);
    }
}

void SkeletonNode::setSkeletonCoMProjectedVisible(bool makeVisible)
{
    if (_debug) {
        std::cerr << "[SkeletonNode] " << (makeVisible ? "Showing " : "Hiding ")
                  << "Projected CoM for " << this->getName() << std::endl;
    }

    if (_skeletonVisuals->getProjectedCenterOfMassTF()) {
        _skeletonVisuals->getProjectedCenterOfMassTF()->setNodeMask(makeVisible ? 0xffffffff : 0x0);
    }
}

void SkeletonNode::setSkeletonRenderMode(renderMode_t renderMode)
{
    switch (renderMode) {
        case RENDER_VISUAL_MESH: {
            for (size_t i=0; i<_bodyNodeGroups.size(); ++i) {
                _bodyNodeGroups[i]->setNodeMask(0xffffffff);
            }
            for (size_t i=0; i<_bodyNodeCollisionMeshGroups.size(); ++i) {
                _bodyNodeCollisionMeshGroups[i]->setNodeMask(0x0);
            }
            break;
        }
        case RENDER_COLLISION_MESH: {
            for (size_t i=0; i<_bodyNodeCollisionMeshGroups.size(); ++i) {
                _bodyNodeCollisionMeshGroups[i]->setNodeMask(0xffffffff);
            }
            for (size_t i=0; i<_bodyNodeGroups.size(); ++i) {
                _bodyNodeGroups[i]->setNodeMask(0x0);
            }
            break;
        }
        case RENDER_WIREFRAME_ON: {
            for(size_t i=0; i<_bodyNodeGroups.size(); ++i) {
                for(size_t j=0; j<_bodyNodeGroups.at(i)->getNumChildren(); ++j) {
                    osgGolems::setWireFrameOn(_bodyNodeGroups.at(i)->getChild(j));
                }
            }
            for (size_t i=0; i<_bodyNodeCollisionMeshGroups.size(); ++i) {
                for (size_t j=0; j<_bodyNodeCollisionMeshGroups.at(i)->getNumChildren(); ++j) {
                    osgGolems::setWireFrameOn(_bodyNodeCollisionMeshGroups.at(i)->getChild(j));
                }
            }
            break;
        }
        case RENDER_WIREFRAME_OFF: {
            for (size_t i=0; i<_bodyNodeGroups.size(); ++i) {
                for (size_t j=0; j<_bodyNodeGroups.at(i)->getNumChildren(); ++j) {
                    osgGolems::setWireFrameOff(_bodyNodeGroups.at(i)->getChild(j));
                }
            }
            for (size_t i=0; i<_bodyNodeCollisionMeshGroups.size(); ++i) {
                for (size_t j=0; j<_bodyNodeCollisionMeshGroups.at(i)->getNumChildren(); ++j) {
                    osgGolems::setWireFrameOff(_bodyNodeCollisionMeshGroups.at(i)->getChild(j));
                }
            }
            break;
        }
    }
}

void SkeletonNode::setBodyNodeTransparency(const dart::dynamics::BodyNode& node, float transparencyValue)
{
    BodyNodeGroupMap::const_iterator it = _bodyNodeGroupMap.find(&node);
    if (it != _bodyNodeGroupMap.end()) {
        osgGolems::setTransparency(_bodyNodeGroupMap.at(&node), transparencyValue);
    }

}


void SkeletonNode::update()
{
    // First update root joint transform, which places the skeleton relative to the world
    _bodyNodeMatrixMap.at(&_rootBodyNode)->setMatrix(osgGolems::eigToOsgMatrix(_rootBodyNode.getTransform()));
    _bodyNodeVisualsMap.at(&_rootBodyNode)->setMatrix(osgGolems::eigToOsgMatrix(_rootBodyNode.getTransform()));

    // Then recursively update all the children of the root body node
    for (int i=0; i<_rootBodyNode.getNumChildBodyNodes(); ++i) {
        _updateRecursively(*_rootBodyNode.getChildBodyNode(i));
    }

    _updateSkeletonVisuals();
}

void SkeletonNode::_updateSkeletonVisuals()
{
    osg::Matrix comTF;
    comTF.makeTranslate(osgGolems::eigToOsgVec3(_rootBodyNode.getSkeleton()->getWorldCOM()));
    if (_skeletonVisuals->getCenterOfMassTF()) {
        _skeletonVisuals->getCenterOfMassTF()->setMatrix(comTF);
    }
    comTF(3,2) = 0.0;
    _skeletonVisuals->getProjectedCenterOfMassTF()->setMatrix(comTF);
}

const dart::dynamics::BodyNode& SkeletonNode::getRootBodyNode()
{
    return _rootBodyNode;
}

void SkeletonNode::_createSkeleton()
{
    // Get rootBodyNode's parent Joint, convert to osg::MatrixTransform,
    // add rootBodyNode to it, and then add child joint
    osg::MatrixTransform* root =  new osg::MatrixTransform(osgGolems::eigToOsgMatrix(_rootBodyNode.getTransform()));
    root->addChild(_makeBodyNodeGroup(_rootBodyNode));
    root->addChild(_makeBodyNodeCollisionMeshGroup(_rootBodyNode));
    this->addChild(root);

    _bodyNodeMatrixMap.insert(std::make_pair(&_rootBodyNode, root));
    _addSkeletonObjectsRecursivley(_rootBodyNode);

    _addSkeletonVisuals();
}

void SkeletonNode::_addSkeletonVisuals()
{
    if (_rootBodyNode.getSkeleton()->getNumBodyNodes() > 1) {
        _skeletonVisuals->addCenterOfMass();
    }
    _skeletonVisuals->addProjectedCenterOfMass();
    _updateSkeletonVisuals();
    this->addChild(_skeletonVisuals);
}

void SkeletonNode::_addSkeletonObjectsRecursivley(const dart::dynamics::BodyNode& bodyNode)
{
    // Add child BodyNodes to parent Joint
    for (int i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
        // Get child BodyNode and add its parent Joint to the grandparent Joint
        dart::dynamics::BodyNode* childBodyNode = bodyNode.getChildBodyNode(i);
        osg::MatrixTransform* childNodeTF = new osg::MatrixTransform(osgGolems::eigToOsgMatrix(childBodyNode->getTransform()));
        childNodeTF->addChild(_makeBodyNodeGroup(*childBodyNode));
        childNodeTF->addChild(_makeBodyNodeCollisionMeshGroup(*childBodyNode));
        this->addChild(childNodeTF);

        _bodyNodeMatrixMap.insert(std::make_pair(childBodyNode, childNodeTF));

        _addSkeletonObjectsRecursivley(*childBodyNode);
    }
}

void SkeletonNode::_updateRecursively(const dart::dynamics::BodyNode& bodyNode)
{
    // Get child node and update its transform. Then get its children and update theirs
    BodyNodeMatrixMap::const_iterator it = _bodyNodeMatrixMap.find(&bodyNode);
    if (it != _bodyNodeMatrixMap.end()) {
        _bodyNodeMatrixMap.at(&bodyNode)->setMatrix(osgGolems::eigToOsgMatrix(bodyNode.getTransform()));
        _bodyNodeVisualsMap.at(&bodyNode)->setMatrix(osgGolems::eigToOsgMatrix(bodyNode.getTransform()));

        for (int i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
            _updateRecursively(*bodyNode.getChildBodyNode(i));
        }
    }
}

osg::Group* SkeletonNode::_makeBodyNodeGroup(const dart::dynamics::BodyNode& node)
{
    // Create osg::Group in std::map b/t BodyNodes and osg::Groups
    _bodyNodeGroupMap.insert(std::make_pair(&node, new osg::Group));

    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    _addVisualizationShapesFromBodyNode(node);

    osgDart::BodyNodeVisuals* visuals = _makeBodyNodeVisuals(node);

    this->addChild(visuals);

    _bodyNodeVisualsMap.insert(std::make_pair(&node, visuals));
    _bodyNodeVisuals.push_back(visuals);

    // Add BodyNode osg::Group to class array
    _bodyNodeGroups.push_back(_bodyNodeGroupMap.at(&node));


    // Return the osg::Group version of the BodyNode
    return _bodyNodeGroupMap.at(&node);
}

osgDart::BodyNodeVisuals* SkeletonNode::_makeBodyNodeVisuals(const dart::dynamics::BodyNode &node)
{
    osgDart::BodyNodeVisuals* visuals = new osgDart::BodyNodeVisuals;

    visuals->setMatrix(osgGolems::eigToOsgMatrix(node.getTransform()));
    visuals->addBodyNodesAxes();
    visuals->getBodyNodeAxesTF()->setNodeMask(0x0);

    if (node.getParentBodyNode() && node.getParentJoint()) {
        if (node.getParentJoint()->getJointType() == dart::dynamics::Joint::REVOLUTE) {
            dart::dynamics::RevoluteJoint* parentJoint =
                    dynamic_cast<dart::dynamics::RevoluteJoint*>(node.getParentJoint());

            Eigen::Quaterniond axisQuat;
            axisQuat.setFromTwoVectors(Eigen::Vector3d(1,0,0), parentJoint->getAxis());
            Eigen::Isometry3d axisTF = Eigen::Isometry3d(axisQuat);

            visuals->addJointAxis();
            visuals->getJointAxisTF()->setMatrix(osgGolems::eigToOsgMatrix(axisTF));
            visuals->setJointAxisColor(osg::Vec4(1,0,1,1));
            visuals->getJointAxisTF()->setNodeMask(0x0);
        }
    }
    return visuals;
}

osg::Group* SkeletonNode::_makeBodyNodeCollisionMeshGroup(const dart::dynamics::BodyNode &node)
{
    osg::ref_ptr<osg::Group> collisionGroup = new osg::Group;

    _bodyNodeCollsionMeshGroupMap.insert(std::make_pair(&node, collisionGroup));

    _addCollisionShapesFromBodyNode(node);

    _bodyNodeCollisionMeshGroups.push_back(collisionGroup);

    collisionGroup.get()->setNodeMask(0x0);

    return collisionGroup.get();
}

void SkeletonNode::_addVisualizationShapesFromBodyNode(const dart::dynamics::BodyNode& node)
{
//    std::cerr << node.getSkeleton()->getName() << ". numShapes: " << node.getNumVisualizationShapes() << std::endl;
    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    for (int i=0; i<node.getNumVisualizationShapes(); ++i) {
        switch (node.getVisualizationShape(i)->getShapeType()) {
            std::cerr << node.getName() << std::endl;
            case dart::dynamics::Shape::BOX:
            case dart::dynamics::Shape::ELLIPSOID:
            case dart::dynamics::Shape::CYLINDER: {
                _bodyNodeGroupMap.at(&node)->addChild(convertShapeToOsgNode(
                                                          node.getVisualizationShape(i)));
                break;
            }
            case dart::dynamics::Shape::MESH: {
                _bodyNodeGroupMap.at(&node)->addChild(convertMeshToOsgNode(
                                                          node.getVisualizationShape(i)));
                 break;
            }
        }
        _bodyNodeGroupMap.at(&node)->getOrCreateStateSet()->setAttribute(new osg::Material);
        osg::Material* mat = (osg::Material*)_bodyNodeGroupMap.at(&node)->
                getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL);
        mat->setDiffuse(osg::Material::FRONT_AND_BACK,
                        osg::Vec4f(osgGolems::eigToOsgVec3(
                                       node.getVisualizationShape(0)->getColor()), 1.0));
        _bodyNodeGroupMap.at(&node)->getOrCreateStateSet()->setAttribute(mat);
        _bodyNodeGroupMap.at(&node)->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc);
    }
}

void SkeletonNode::_addCollisionShapesFromBodyNode(const dart::dynamics::BodyNode& node)
{
//    std::cerr << "[SkeletonNode] " << node.getName() << " has " << node.getNumCollisionShapes() << " collision shapes" << std::endl;
    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    for (int i=0; i<node.getNumCollisionShapes(); ++i) {
        switch (node.getCollisionShape(i)->getShapeType()) {
            case dart::dynamics::Shape::BOX:
            case dart::dynamics::Shape::ELLIPSOID:
            case dart::dynamics::Shape::CYLINDER: {
                _bodyNodeCollsionMeshGroupMap.at(&node)->addChild(convertShapeToOsgNode(node.getCollisionShape(i)));
                break;
            }
            case dart::dynamics::Shape::MESH: {
                _bodyNodeCollsionMeshGroupMap.at(&node)->addChild(convertMeshToOsgNode(node.getCollisionShape(i)));
                 break;
            }
        }
    }
}
