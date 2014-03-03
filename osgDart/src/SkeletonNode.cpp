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

#include "SkeletonNode.h"
#include "osgAssimpSceneReader.h"
#include <assimp/scene.h>
#include <dart/dynamics/MeshShape.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include "osgUtils.h"

using namespace dart;
using namespace osgDart;

void SkeletonNode::moveJoint()
{
//    std::cerr << "Joint name: " << _rootBodyNode->getChildBodyNode(0)->getParentJoint()->getName() << std::endl;
}

SkeletonNode::SkeletonNode(dynamics::Skeleton* robot, float axisLength) :
    _axisLength(axisLength),
    _rootBodyNode(NULL)
{
    _createSkeletonFromRootBodyNode(robot->getRootBodyNode());
    moveJoint();
}

void SkeletonNode::update()
{
    // First update root joint transform, which places the skeleton relative to the world
    _jointMatrixMap[_rootBodyNode->getParentJoint()]->setMatrix(osgGolems::eigToOsgMatrix(_rootBodyNode->getParentJoint()->getTransformFromParentBodyNode()));

    // Then recursively update all the children of the root body node
    for(int i=0; i<_rootBodyNode->getNumChildBodyNodes(); ++i) {
        _updateRecursively(_rootBodyNode->getChildBodyNode(i));
    }
}

dynamics::BodyNode* SkeletonNode::getRootBodyNode()
{
    return _rootBodyNode;
}

void SkeletonNode::_createSkeletonFromRootBodyNode(dynamics::BodyNode* rootBodyNode)
{
    _rootBodyNode = rootBodyNode;

    // Get rootBodyNode's parent Joint, convert to osg::MatrixTransform,
    // add rootBodyNode to it, and then add child joint
    osg::MatrixTransform* root = _placeRootOfSkeletonInWorld(rootBodyNode);
    this->addChild(root);

    osg::MatrixTransform* rootTF = _makeJointNode(rootBodyNode->getParentJoint());
    root->addChild(rootTF);

    _addSkeletonObjectsRecursivley(rootTF, rootBodyNode);

}

osg::MatrixTransform* SkeletonNode::_placeRootOfSkeletonInWorld(dynamics::BodyNode* rootBodyNode)
{
    osg::Matrix rootMatrix = osgGolems::eigToOsgMatrix(rootBodyNode->getWorldTransform());
    osg::MatrixTransform* rootTF = new osg::MatrixTransform();
    rootTF->setMatrix(rootMatrix);
    return rootTF;
}

void SkeletonNode::_addSkeletonObjectsRecursivley(osg::MatrixTransform* jointTF, dynamics::BodyNode* bodyNode)
{
    // For every child node of the bodyNode, add its parent joint's TF to the bodyNode's parent joint's TF
    // and add a TF for the child node to its parent joint's TF.
    // Add parent joint with its TF from its parent body node

    // Add body node with position as Local COM relative to its parent joint
    osg::MatrixTransform* bodyNodeTF = new osg::MatrixTransform;
    bodyNodeTF->setMatrix(_getBodyNodeMatrix(bodyNode));
    jointTF->addChild(bodyNodeTF);
    bodyNodeTF->addChild(_makeBodyNodeGroup(bodyNode));

    DEBUG("Added: \n\tJoint: " << bodyNode->getParentJoint()->getName()
          << "\n\tBody: " << bodyNode->getName()
          << "\n\tChildren: ");
//            << " at \n" << bodyNode->getParentJoint()->getTransformFromParentBodyNode().matrix()
//            << "\n\tBody " << bodyNode->getName() << " at " << bodyNode->getLocalCOM().transpose()
//            << "\nChilds " << bodyNode->getNumChildBodyNodes());

    for(int i=0; i<bodyNode->getNumChildBodyNodes(); ++i) {
        DEBUG("\t    " << bodyNode->getChildBodyNode(i)->getName());

    }

    // Add child BodyNodes to parent Joint
    for(int i=0; i<bodyNode->getNumChildBodyNodes(); ++i) {
        // Get child BodyNode and add its parent Joint to the grandparent Joint
        dynamics::BodyNode* childBodyNode = bodyNode->getChildBodyNode(i);
        osg::MatrixTransform* joint2TF = _makeJointNode(childBodyNode->getParentJoint());
        jointTF->addChild(joint2TF);

        // Create TF for child BodyNode and add BodyNode mesh to it
        osg::MatrixTransform* childBodyNodeTF = new osg::MatrixTransform;
        childBodyNodeTF->setMatrix(_getBodyNodeMatrix(childBodyNode));
        joint2TF->addChild(childBodyNodeTF);
        childBodyNodeTF->addChild(_makeBodyNodeGroup(childBodyNode));

        _addSkeletonObjectsRecursivley(joint2TF, childBodyNode);
    }
}

void SkeletonNode::_updateRecursively(dynamics::BodyNode* bodyNode)
{
    // Get child node and update its transform. Then get its children and update theirs
    JointMatrixMap::const_iterator m = _jointMatrixMap.find(bodyNode->getParentJoint());
    if(m != _jointMatrixMap.end()) {
        _jointMatrixMap[bodyNode->getParentJoint()]->setMatrix(osgGolems::eigToOsgMatrix(bodyNode->getParentJoint()->getLocalTransform()));

        for(size_t i=0; i<bodyNode->getNumChildBodyNodes(); ++i) {
            _updateRecursively(bodyNode->getChildBodyNode(i));
        }

    }
}

osg::Group* SkeletonNode::_makeBodyNodeGroup(dynamics::BodyNode* node)
{
    // Create osg::Group in std::map b/t BodyNodes and osg::Groups
    _bodyNodeGroupMap[node] = new osg::Group;

    // Loop through visualization shapes and create nodes and add them to a MatrixTransform
    for(int i=0; i<node->getNumVisualizationShapes(); ++i) {
        dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(i);
        const aiScene* aiscene = meshShape->getMesh();
        _bodyNodeGroupMap[node]->addChild(osgAssimpSceneReader::traverseAIScene(aiscene, aiscene->mRootNode));
    }

    // Add BodyNode osg::Group to class array, and set data variance to dynamic
    _bodyNodes.push_back(_bodyNodeGroupMap[node]);

    // Return the osg::Group version of the BodyNode
    return _bodyNodeGroupMap[node];
}

osg::Matrix SkeletonNode::_getBodyNodeMatrix(dynamics::BodyNode* bodyNode)
{
    osg::Matrix m;
    m.makeTranslate(osgGolems::eigToOsgVec(bodyNode->getLocalCOM()));
    return m;
}

osg::MatrixTransform* SkeletonNode::_makeJointNode(dynamics::Joint* joint)
{
    // Create osg::MatrixTransform, set it using the Joint's TF from its parent BodyNode
    _jointMatrixMap[joint] = new osg::MatrixTransform;
    _jointMatrixMap[joint]->setMatrix(osgGolems::eigToOsgMatrix(joint->getTransformFromParentBodyNode()));

    // Add Joint osg::MatrixTransform to class array, and set data variance to dynamics
    _joints.push_back(_jointMatrixMap[joint]);

    // Return the osg::MatrixTransform version of the Joint
    return _jointMatrixMap[joint];
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
