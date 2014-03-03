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
#include "../../osgNodes/include/Axes.h"

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

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

/// Definition of type JointMatrixMap, which maps dart::dynamics::Joint* to osg::MatrixTransform*
typedef std::map<dynamics::Joint*, osg::ref_ptr<osg::MatrixTransform> > JointMatrixMap;

/// Definition of type BodyNodeGroupMap, which maps dart::dynamics::BodyNode* to osg::Group*
typedef std::map<dynamics::BodyNode*, osg::ref_ptr<osg::Group> > BodyNodeGroupMap;

/**
 * \class SkeletonNode SkeletonNode.h
 * \brief Class which inherits osg::Group
 */
class SkeletonNode : public osg::Group
{
public:

    //---------------------------------------------------------------
    //                       PUBLIC FUNCTIONS
    //---------------------------------------------------------------

    /**
     * \brief Constructor for SkeletonNode
     */
    SkeletonNode(dynamics::Skeleton* robot, float axisLength=0.2);

    /**
     * \brief Update SkeletonNode based on Skeleton transforms
     */
    void update();

protected:

    //---------------------------------------------------------------
    //                     PROTECTED FUNCTIONS
    //---------------------------------------------------------------
    /**
     * \brief Update the skeleton
     */
    void _createSkeletonFromRootBodyNode(dynamics::BodyNode *rootBodyNode);

    /**
     * \brief Place the root of the Skeleton in the world using the passed in BodyNode's
     * world transform
     * \param rootBodyNode The dart::dynamics::BodyNode root to place in the world
     * \return osg::MatrixTransform* This is returned in order to add children nodes to it.
     */
    osg::MatrixTransform* _placeRootOfSkeletonInWorld(dynamics::BodyNode *rootBodyNode);

    /**
     * \brief Pass in osg::MatrixTransform corresponding to parent Joint and its BodyNode
     * and it will recursively add osg::MatrixTransforms and Geode for all the children
     */
    void _addSkeletonObjectsRecursivley(osg::MatrixTransform* jointTF, dynamics::BodyNode *bodyNode);

    /**
     * \brief Create osg::Group* object from a dart::dynamics::BodyNode passed in by reference.
     * It generates the shape information in an OpenSceneGraph format, looping through the BodyNode's
     * visualization shapes and converting them from aiScene objects to osg::Node objects and adding
     * them as children to the BodyNode osg::Group.
     * \param node dart::dynamics::BodyNode of which to make an osg::Group*
     * \return osg::Group* The osg::Group* corresponding the dart::dynamics::BodyNode
     */
    osg::Group* _makeBodyNodeGroup(dynamics::BodyNode *node);

    /**
     * \brief Create osg::MatrixTransform* from a dart::dynamics::Joint passed in by reference.
     * The transform corresponds to the transform from its parent BodyNode to itself.
     * \param joint dart::dynamics::Joint for which to create an osg::MatrixTransform*
     * \return osg::MatrixTransform pointer corresponding to the dart::dynamics::Joint passed in
     */
    osg::MatrixTransform* _makeJointNode(dynamics::Joint *joint);

    /**
     * \brief Get the local center of mass of the BodyNode relative to its parent joint,
     * used to place the BodyNode relative to its parent joint in the visualization.
     * The BodyNode is passed in by reference.
     * \param node The dynamics::BodyNode of which to get the local center of mass
     * \return osg::Matrix defining the local center of mass of the BodyNode relative to its parent joint
     */
    osg::Matrix _getBodyNodeMatrix(dynamics::BodyNode *node);

    /**
     * \brief Update SkeletonNode recursively based on Skeleton transforms
     */
    void _updateRecursively(dynamics::BodyNode *bodyNode);

    /**
     * \brief Get root body node
     */

    void moveJoint();

    dynamics::BodyNode* getRootBodyNode();


    //---------------------------------------------------------------
    //                    PROTECTED VARIABLES
    //---------------------------------------------------------------

    /// Array of osg::Group pointers for the dart::dynamics::BodyNode visualization objects
    std::vector<osg::ref_ptr<osg::Group> > _bodyNodes;

    /// Array of osg::MatrixTransform pointers for the dart::dynamics::Joint visualization objects
    std::vector<osg::ref_ptr<osg::MatrixTransform> > _joints;

    /// Map from dart::dynamics::Joint* to osg::MatrixTransform*
    JointMatrixMap _jointMatrixMap;

    /// Map from dart::dynamics::BodyNode* to osg::Group*
    BodyNodeGroupMap _bodyNodeGroupMap;

    dynamics::BodyNode* _rootBodyNode;

    /// Length of joint axes visualization
    float _axisLength;
};

} // end osgDart namespace

#endif // SKELETONNODE_H
