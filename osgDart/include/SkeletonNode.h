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
typedef std::map<dynamics::BodyNode*, osg::ref_ptr<osg::MatrixTransform> > BodyNodeMatrixMap;

/// Definition of type BodyNodeGroupMap, which maps dart::dynamics::BodyNode* to osg::Group*
typedef std::map<dynamics::BodyNode*, osg::ref_ptr<osg::Group> > BodyNodeGroupMap;

/**
 * \class SkeletonNode SkeletonNode.h
 * \brief Class which inherits osg::Group
 */
class SkeletonNode : public osg::Switch
{
public:

    //---------------------------------------------------------------
    //                       PUBLIC FUNCTIONS
    //---------------------------------------------------------------

    /**
     * \brief Constructor for SkeletonNode
     * \param skeleton A dart::dynamics::Skeleton pointer from which to create the osg skeleton
     * \param axisLength Length of the joint axes (meters)
     * \param debug Debug flag for printing debug statements to standard error
     */
    SkeletonNode(dynamics::Skeleton* skeleton, float axisLength=0.2, bool debug=false);

    /**
     * \brief Destructor for SkeletonNode
     */
    ~SkeletonNode();

    /**
     * \brief Update SkeletonNode MatrixTransforms based on the dart::Skeleton BodyNode world transforms.
     * This is done recursively by traversing through the Skeleton bodyNodes and their children.
     * \return void
     */
    void update();

protected:

    //---------------------------------------------------------------
    //                     PROTECTED FUNCTIONS
    //---------------------------------------------------------------
    /**
     * \brief Create the osg skeleton from the root BodyNode by traversing its children and
     * creating osg::Nodes for each physical BodyNode and putting them inside a osg::MatrixTransform with
     * it transform set to the BodyNode's world transform.
     * \param rootBodyNode The root of the skeleton
     * \return void
     */
    void _createSkeletonFromRootBodyNode(dynamics::BodyNode* rootBodyNode);

    /**
     * \brief Recursively build osg skeleton starting with a BodyNode and recursively creating its
     * children. An osg::MatrixTransform get created whose transform corresponds to the BodyNode's
     * world transform and then an osg::Group node gets created from the BodyNode's shape information
     * and get put inside the osg::MatrixTransform. Finally the osg::MatrixTransform gets added as a child
     * to the root node of the SkeletonNode
     * and it will recursively add osg::MatrixTransforms and Geode for all the children
     * \param bodyNode The BodyNode to add to the osg skeleton along with its children
     */
    void _addSkeletonObjectsRecursivley(dynamics::BodyNode* bodyNode);

    /**
     * \brief Create osg::Group* object from a dart::dynamics::BodyNode passed in by reference.
     * It generates the shape information in an OpenSceneGraph format, looping through the BodyNode's
     * visualization shapes and converting them from aiScene objects to osg::Node objects and adding
     * them as children to the BodyNode osg::Group.
     * \param node dart::dynamics::BodyNode of which to make an osg::Group*
     * \return osg::Group* The osg::Group* corresponding the dart::dynamics::BodyNode
     */
    osg::Group* _makeBodyNodeGroup(dynamics::BodyNode* node);

    /**
     * \brief Convert BodyNode shapes to osg shapes
     * \param node BodyNode to get shapes from
     * \return void
     */
    void _addShapesFromBodyNode(dynamics::BodyNode* node);

    /**
     * \brief Update SkeletonNode recursively based on Skeleton transforms. This traverses through each
     * BodyNode and its child BodyNodes and updates corresponding osg::MatrixTransform based on their
     * current world transform.
     * \param bodyNode BodyNode used to update the osg::MatrixTransform and its children
     * \return void
     */
    void _updateRecursively(dynamics::BodyNode* bodyNode);

    /**
     * \brief Get root body node
     * \return dynamics::BodyNode pointer to the root body node
     */
    dynamics::BodyNode* getRootBodyNode();


    //---------------------------------------------------------------
    //                    PROTECTED VARIABLES
    //---------------------------------------------------------------

    /// Root BodyNode
    dynamics::BodyNode* _rootBodyNode;

    /// Array of osg::Group pointers for the dart::dynamics::BodyNode visualization objects
    std::vector<osg::ref_ptr<osg::Group> > _bodyNodes;

    /// Map from dart::dynamics::Joint* to osg::MatrixTransform*
    BodyNodeMatrixMap _bodyNodeMatrixMap;

    /// Map from dart::dynamics::BodyNode* to osg::Group*
    BodyNodeGroupMap _bodyNodeGroupMap;

    /// Length of joint axes visualization
    float _axisLength;

    /// Debug variable for whether or not to print debug output
    const bool _debug;
};

} // end osgDart namespace

#endif // SKELETONNODE_H
