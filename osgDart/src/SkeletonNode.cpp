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
#include "DartVisual.h"

// DART includes
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/EllipsoidShape.h>
#include <dart/dynamics/CylinderShape.h>
#include <dart/dynamics/MeshShape.h>

// OpenSceneGraph includes
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>

using namespace dart;
using namespace osgDart;

SkeletonNode::SkeletonNode(const dynamics::Skeleton &skeleton, float axisLength, bool debug) :
    _axisLength(axisLength),
    _rootBodyNode(*skeleton.getRootBodyNode()),
    _debug(debug)
{
    _createSkeleton();
}

SkeletonNode::~SkeletonNode()
{

}

void SkeletonNode::update()
{
    // First update root joint transform, which places the skeleton relative to the world
    _bodyNodeMatrixMap.at(&_rootBodyNode)->setMatrix(osgGolems::eigToOsgMatrix(_rootBodyNode.getWorldTransform()));

    // Then recursively update all the children of the root body node
    for(int i=0; i<_rootBodyNode.getNumChildBodyNodes(); ++i) {
        _updateRecursively(*_rootBodyNode.getChildBodyNode(i));
    }
}

const dynamics::BodyNode& SkeletonNode::getRootBodyNode()
{
    return _rootBodyNode;
}

void SkeletonNode::_createSkeleton()
{
    // Get rootBodyNode's parent Joint, convert to osg::MatrixTransform,
    // add rootBodyNode to it, and then add child joint
    osg::MatrixTransform* root =  new osg::MatrixTransform(osgGolems::eigToOsgMatrix(_rootBodyNode.getWorldTransform()));
    root->addChild(_makeBodyNodeGroup(_rootBodyNode));
    this->addChild(root);

    _bodyNodeMatrixMap.insert(std::make_pair(&_rootBodyNode, root));
    _addSkeletonObjectsRecursivley(_rootBodyNode);

}

void SkeletonNode::_addSkeletonObjectsRecursivley(const dynamics::BodyNode& bodyNode)
{
    // Add child BodyNodes to parent Joint
    for(int i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
        // Get child BodyNode and add its parent Joint to the grandparent Joint
        dynamics::BodyNode* childBodyNode = bodyNode.getChildBodyNode(i);
        osg::MatrixTransform* childNodeTF = new osg::MatrixTransform(osgGolems::eigToOsgMatrix(childBodyNode->getWorldTransform()));
        childNodeTF->addChild(_makeBodyNodeGroup(*childBodyNode));
        this->addChild(childNodeTF);

        _bodyNodeMatrixMap.insert(std::make_pair(childBodyNode, childNodeTF));

        _addSkeletonObjectsRecursivley(*childBodyNode);
    }
}

void SkeletonNode::_updateRecursively(const dynamics::BodyNode& bodyNode)
{
    // Get child node and update its transform. Then get its children and update theirs
    BodyNodeMatrixMap::const_iterator m = _bodyNodeMatrixMap.find(&bodyNode);
    if(m != _bodyNodeMatrixMap.end()) {
        _bodyNodeMatrixMap.at(&bodyNode)->setMatrix(osgGolems::eigToOsgMatrix(bodyNode.getWorldTransform()));

        for(size_t i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
            _updateRecursively(*bodyNode.getChildBodyNode(i));
        }

    }
}

osg::Group* SkeletonNode::_makeBodyNodeGroup(const dynamics::BodyNode& node)
{
    // Create osg::Group in std::map b/t BodyNodes and osg::Groups
    _bodyNodeGroupMap.insert(std::make_pair(&node, new osg::Group));

    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    _addShapesFromBodyNode(node);
    osgDart::DartVisuals* viz = new osgDart::DartVisuals;
//    viz->getJointAxisTF()->setMatrix(osgGolems::eigToOsgMatrix(node.getParentJoint();
    _bodyNodeGroupMap.at(&node)->addChild(viz);

    // Add BodyNode osg::Group to class array, and set data variance to dynamic
    _bodyNodes.push_back(_bodyNodeGroupMap.at(&node));


    // Return the osg::Group version of the BodyNode
    return _bodyNodeGroupMap.at(&node);
}

void SkeletonNode::_addShapesFromBodyNode(const dynamics::BodyNode& node)
{
    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    for(int i=0; i<node.getNumVisualizationShapes(); ++i) {
        switch (node.getVisualizationShape(i)->getShapeType()) {
            case dynamics::Shape::BOX:
            case dynamics::Shape::ELLIPSOID:
            case dynamics::Shape::CYLINDER: {
                _bodyNodeGroupMap.at(&node)->addChild(convertShapeToOsgNode(node.getVisualizationShape(i)));
                break;
            }
            case dynamics::Shape::MESH: {
                _bodyNodeGroupMap.at(&node)->addChild(convertMeshToOsgNode(node.getVisualizationShape(i)));
                 break;
            }
        }
    }
}


/**
                          Skeleton              osg::Group
                             |
                           Joint                osg::MatrixTransfrom
                  ----------------------
                  |                    |
              BodyNode              BodyNode    osg::Group
                  |                    |
                Joint                Joint      osg::MatrixTransform
                  |                    |
              BodyNode              BodyNode    osg::Group

Make group from rootBodyNode.


*/
