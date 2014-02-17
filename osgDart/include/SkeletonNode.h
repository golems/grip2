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

/**
 * \file SkeletonNode.h
 * \brief OpenSceneGraph osg::Group subclass for dart::dynamics::skeleton objects
 */

#ifndef SKELETONNODE_H
#define SKELETONNODE_H

// Grip includes
#include "Axes.h"

// Dart includes
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/CullFace>

using namespace dart;

namespace osgDart {

typedef std::map<dynamics::Joint*, osg::MatrixTransform*> JointMatrixMap;
typedef std::map<dynamics::BodyNode*, osg::Group*> BodyNodeGroupMap;

/**
 * \class SkeletonNode SkeletonNode.h
 * \brief Class which inherits osg::Group
 */
class SkeletonNode : public osg::Group
{
public:

    /**
     * \brief Constructor for SkeletonNode
     */
    SkeletonNode(dynamics::Skeleton& robot, float axisLength=0.2);

protected:

    //---------------------------------------------------
    //           PROTECTED MEMBER FUNCTIONS
    //---------------------------------------------------
    /**
     * \brief Update the skeleton
     */
    void _createSkeletonFromRootBodyNode(dynamics::BodyNode& rootBodyNode);

    void _recursiveUpdate(osg::MatrixTransform* rootTF, dynamics::BodyNode& bodyNode);

    osg::MatrixTransform* _addRootJointNode(dynamics::Joint& rootJoint);

    /**
     * \brief Create osg::Geode object from DART BodyNode
     */
    osg::Group* _makeBodyNodeGeode(dynamics::BodyNode& node);

    /**
     * \brief Create osg::MatrixTransform from DART Joint
     */
    osg::MatrixTransform* _makeJointNode(dynamics::Joint& joint);


    //---------------------------------------------------
    //           PROTECTED MEMBER VARIABLES
    //---------------------------------------------------
    std::vector<osg::Geode*> _bodyNodes;
    std::vector<osg::MatrixTransform*> _joints;

    JointMatrixMap _jointMatrixMap;
    BodyNodeGroupMap _bodyNodeGroupMap;

    float _axisLength;
};

} // end osgDart namespace

#endif // SKELETONNODE_H
