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
    _createSkeletonFromRootBodyNode(*robot.getBodyNode(0));
}

void SkeletonNode::_createSkeletonFromRootBodyNode(dynamics::BodyNode& rootBodyNode)
{
    // Get rootBodyNode's parent Joint, convert to osg::MatrixTransform,
    // add rootBodyNode to it, and then add child joint
    osg::MatrixTransform* rootTF = _addRootJointNode(*rootBodyNode.getParentJoint());
    this->addChild(rootTF);

    _recursiveUpdate(rootTF, rootBodyNode);

}

void SkeletonNode::_recursiveUpdate(osg::MatrixTransform* rootTF, dynamics::BodyNode& bodyNode)
{
    osg::MatrixTransform* bodyNodeTF = new osg::MatrixTransform;
    osg::Matrix m;
    m.makeTranslate(osgUtils::eigToOsgVec(bodyNode.getLocalCOM()));
    bodyNodeTF->setMatrix(m);
    rootTF->addChild(bodyNodeTF);
    bodyNodeTF->addChild(_makeBodyNodeGeode(bodyNode));
    std::cerr << "Added " << bodyNode.getName() << std::endl;

//    dynamics::BodyNode* node = bodyNode.getChildBodyNode(0);
//    osg::MatrixTransform* tf = new osg::MatrixTransform;

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
    osg::Group* bodyNodeGroup = new osg::Group;

    // Loop through visualization shapes and create nodes and add them to
    for(int i=0; i<node.getNumVisualizationShapes(); ++i) {
        dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node.getVisualizationShape(i);
        const aiScene* aiscene = meshShape->getMesh();
        bodyNodeGroup->addChild(osgAssimpSceneReader::traverseAIScene(aiscene, aiscene->mRootNode));
    }

    // Return the osg::Group version of the BodyNode
    return bodyNodeGroup;
}

osg::MatrixTransform* SkeletonNode::_makeJointNode(dynamics::Joint& joint)
{
    // Create osg::MatrixTransform, set it using the Joint's TF from its parent BodyNode
    _jointMatrixMap[&joint] = new osg::MatrixTransform;
    _jointMatrixMap[&joint]->setMatrix(osgUtils::eigToOsgMatrix(joint.getTransformFromParentBodyNode()));
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
