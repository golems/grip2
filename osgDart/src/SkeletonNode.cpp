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
#include "SkeletonNode.h"
#include "osgAssimpSceneReader.h"
#include <assimp/scene.h>
#include <dart/dynamics/MeshShape.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include "osgUtils.h"

using namespace dart;
using namespace osgDart;

SkeletonNode::SkeletonNode(dynamics::Skeleton& robot, float axisLength) :
    _axisLength(axisLength)
{
    _createSkeletonFromRootBodyNode(*robot.getRootBodyNode());
}

void SkeletonNode::_createSkeletonFromRootBodyNode(dynamics::BodyNode& rootBodyNode)
{
    // Get rootBodyNode's parent Joint, convert to osg::MatrixTransform,
    // add rootBodyNode to it, and then add child joint
    osg::MatrixTransform* rootTF = _addRootJointNode(*rootBodyNode.getParentJoint());
    this->addChild(rootTF);

    _recursiveUpdate(rootTF, rootBodyNode);

}

void SkeletonNode::_recursiveUpdate(osg::MatrixTransform* jointTF, dynamics::BodyNode& bodyNode)
{
    // For every child node of the bodyNode, add its parent joint's TF to the bodyNode's parent joint's TF
    // and add a TF for the child node to its parent joint's TF.
    // Add parent joint with its TF from its parent body node

    // Add body node with position as Local COM relative to its parent joint
    osg::MatrixTransform* bodyNodeTF = new osg::MatrixTransform;
    bodyNodeTF->setMatrix(_getBodyNodeMatrix(bodyNode));
    jointTF->addChild(bodyNodeTF);
    bodyNodeTF->addChild(_makeBodyNodeGeode(bodyNode));

    std::cerr << "Added \n\tJoint " << bodyNode.getParentJoint()->getName() << " at \n" << bodyNode.getParentJoint()->getTransformFromParentBodyNode().matrix()
              << "\n\tBody " << bodyNode.getName() << " at " << bodyNode.getLocalCOM().transpose()
              << "\n\tChilds " << bodyNode.getNumChildBodyNodes()
              << std::endl;
    for(int i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
        std::cerr << "\t\t" << bodyNode.getChildBodyNode(i)->getName() << std::endl;

    }

    // Add child BodyNodes to parent Joint
    for(int i=0; i<bodyNode.getNumChildBodyNodes(); ++i) {
        // Get child BodyNode and add its parent Joint to the grandparent Joint
        dynamics::BodyNode childBodyNode = *bodyNode.getChildBodyNode(i);
        osg::MatrixTransform* joint2TF = _makeJointNode(*childBodyNode.getParentJoint());
        jointTF->addChild(joint2TF);

        // Create TF for child BodyNode and add BodyNode mesh to it
        osg::MatrixTransform* childBodyNodeTF = new osg::MatrixTransform;
        childBodyNodeTF->setMatrix(_getBodyNodeMatrix(childBodyNode));
        joint2TF->addChild(childBodyNodeTF);
        childBodyNodeTF->addChild(_makeBodyNodeGeode(childBodyNode));
        std::cerr << "Added \n\tJoint " << childBodyNode.getParentJoint()->getName() << " at \n" << childBodyNode.getParentJoint()->getTransformFromParentBodyNode().matrix()
                  << "\n\tBody " << childBodyNode.getName() << " at " << childBodyNode.getLocalCOM().transpose() << std::endl;
        _recursiveUpdate(joint2TF, childBodyNode);
    }
}

osg::MatrixTransform* SkeletonNode::_addRootJointNode(dynamics::Joint& rootJoint)
{
    _jointMatrixMap[&rootJoint] = _makeJointNode(rootJoint);
    this->addChild(_jointMatrixMap[&rootJoint]);
    return _jointMatrixMap[&rootJoint];
}


osg::Group* SkeletonNode::_makeBodyNodeGeode(dynamics::BodyNode& node)
{
    // Create osg::Group in std::map b/t BodyNodes and osg::Groups
    _bodyNodeGroupMap[&node] = new osg::Group;

    // Loop through visualization shapes and create nodes and add them to
    for(int i=0; i<node.getNumVisualizationShapes(); ++i) {
        dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node.getVisualizationShape(i);
        const aiScene* aiscene = meshShape->getMesh();
        _bodyNodeGroupMap[&node]->addChild(osgAssimpSceneReader::traverseAIScene(aiscene, aiscene->mRootNode));
    }

    // Add BodyNode osg::Group to class array, and set data variance to dynamic
    _bodyNodes.push_back(_bodyNodeGroupMap[&node]);
    _bodyNodeGroupMap[&node]->setDataVariance(osg::Object::DYNAMIC);

    // Return the osg::Group version of the BodyNode
    return _bodyNodeGroupMap[&node];
}

osg::Matrix SkeletonNode::_getBodyNodeMatrix(dynamics::BodyNode& bodyNode)
{
    osg::Matrix m;
    m.makeTranslate(osgUtils::eigToOsgVec(bodyNode.getLocalCOM()));
    return m;
}

osg::MatrixTransform* SkeletonNode::_makeJointNode(dynamics::Joint& joint)
{
    // Create osg::MatrixTransform, set it using the Joint's TF from its parent BodyNode
    _jointMatrixMap[&joint] = new osg::MatrixTransform;
    _jointMatrixMap[&joint]->setMatrix(osgUtils::eigToOsgMatrix(joint.getTransformFromParentBodyNode()));

    // Add Joint osg::MatrixTransform to class array, and set data variance to dynamics
    _joints.push_back(_jointMatrixMap[&joint]);
    _jointMatrixMap[&joint]->setDataVariance(osg::Object::DYNAMIC);

    // Return the osg::MatrixTransform version of the Joint
    return _jointMatrixMap[&joint];
}


/**
                          Skeleton              osg::Group
                             |
                           Joint                osg::MatrixTransfrom
                  ----------------------
                  |                    |
              BodyNode              BodyNode    osg::Geode
                  |                    |
                Joint                Joint      osg::MatrixTransform
                  |                    |
              BodyNode              BodyNode    osg::Geode

Make group from rootBodyNode.


*/
