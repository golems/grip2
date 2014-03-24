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
#include "BodyNodeVisuals.h"
#include "SkeletonVisuals.h"

// Dart includes
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/CullFace>

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

/// Definition of type JointMatrixMap, which maps dart::dynamics::Joint* to osg::MatrixTransform*
typedef std::map<const dart::dynamics::BodyNode*, osg::ref_ptr<osg::MatrixTransform> > BodyNodeMatrixMap;

/// Definition of type BodyNodeGroupMap, which maps dart::dynamics::BodyNode* to osg::Group*
typedef std::map<const dart::dynamics::BodyNode*, osg::ref_ptr<osg::Group> > BodyNodeGroupMap;

/// Definition of type BodyNodeGroupMap, which maps dart::dynamics::BodyNode* to osg::Group*
typedef std::map<const dart::dynamics::BodyNode*, osg::ref_ptr<osgDart::BodyNodeVisuals> > BodyNodeVisualsMap;

/**
 * \enum renderMode_t
 * \brief Render options for the skeleton
 */
typedef enum {
    RENDER_VISUAL_MESH,
    RENDER_COLLISION_MESH,
    RENDER_WIREFRAME_ON,
    RENDER_WIREFRAME_OFF
} renderMode_t;

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
     * \param skeleton A dart::dynamics::Skeleton pointer from which to create the osg skeleton
     * \param axisLength Length of the joint axes (meters)
     * \param debug Debug flag for printing debug statements to standard error
     */
    SkeletonNode(const dart::dynamics::Skeleton& skeleton, bool debug=false);

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

    /**
     * \brief Shows or hides the individual joint axes. This applies to skeletons with more than one
     * link. A single line with an arrow is displayed representing the axis of rotation of the joint.
     * This currently only applies to revolute joints.
     * \param makeVisible Whether or not to visualize the joint axes
     * \return void
     */
    void setJointAxesVisible(bool makeVisible=false);

    /**
     * \brief Shows or hides the individual link frames, represented by x,y,z axes using the color
     * scheme red, green, blue, respectively. Currently this function doesn't account for different
     * sized objects so axes may not be visible for larger objects.
     * \param makeVisible Whether or not to visualize the link axes
     * \return void
     */
    void setBodyNodeAxesVisible(bool makeVisible=false);

    /**
     * \brief Shows or hides the individual link center of masses, represented by a sphere.
     * Currently not implemented.
     * \param makeVisible Whether or not to visualize link center of masses
     * \return void
     */
    void setBodyNodeCoMVisible(bool makeVisible=false);

    /**
     * \brief Shows or hides the skeleton's center of mass, represented by a sphere.
     * \param makeVisible Whether or not to visualize the skeleton's center of mass
     * \return void
     */
    void setSkeletonCoMVisible(bool makeVisible=false);

    /**
     * \brief Shows or hides the skeleton's projected center of mass, represented by a circle on
     * the ground.
     * \param makeVisible Whether or not to visualize the skeleton's projected center of mass
     * \return void
     */
    void setSkeletonCoMProjectedVisible(bool makeVisible=false);

    /**
     * \brief Set the render mode of the Skeleton.
     * \param renderMode The render mode specified by the enumeration, renderMode_t
     * \return void
     */
    void setSkeletonRenderMode(renderMode_t renderMode);

    /**
     * \brief Sets the transparency value of the specified BodyNode
     * \param node dart::dynamics::BodyNode of which to set the transparency
     * \param transparencyValue New transparency value for the node
     * \return void
     */
    void setBodyNodeTransparency(const dart::dynamics::BodyNode& node, float transparencyValue);

    /**
     * \brief Get root body node
     * \return dart::dynamics::BodyNode pointer to the root body node
     */
    const dart::dynamics::BodyNode& getRootBodyNode();

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
    void _createSkeleton();

    /**
     * \brief Add visual elements of skeleton that can be toggled on or off, such as the
     * center of mass.
     * \return void
     */
    void _addSkeletonVisuals();

    /**
     * \brief Recursively build osg skeleton starting with a BodyNode and recursively creating its
     * children. An osg::MatrixTransform get created whose transform corresponds to the BodyNode's
     * world transform and then an osg::Group node gets created from the BodyNode's shape information
     * and get put inside the osg::MatrixTransform. Finally the osg::MatrixTransform gets added as a child
     * to the root node of the SkeletonNode
     * and it will recursively add osg::MatrixTransforms and Geode for all the children
     * \param bodyNode The BodyNode to add to the osg skeleton along with its children
     */
    void _addSkeletonObjectsRecursivley(const dart::dynamics::BodyNode& bodyNode);

    /**
     * \brief Create osg::Group* object from a dart::dynamics::BodyNode passed in by reference.
     * It generates the shape information in an OpenSceneGraph format, looping through the BodyNode's
     * visualization shapes and converting them from aiScene objects to osg::Node objects and adding
     * them as children to the BodyNode osg::Group.
     * \param node dart::dynamics::BodyNode of which to make an osg::Group*
     * \return osg::Group* The osg::Group* corresponding to the dart::dynamics::BodyNode
     */
    osg::Group* _makeBodyNodeGroup(const dart::dynamics::BodyNode& node);

    /**
     * \brief Create osg::Group* object from a dart::dynamics::BodyNode passed in by reference.
     * It generates the shape information in an OpenSceneGraph format, looping through the BodyNode's
     * collision shapes and converting them from aiScene objects to osg::Node objects and adding
     * them as children to the BodyNode osg::Group.
     * \param node const dart::dynamics::BodyNode reference of which to make an osg::Group*
     * \return osg::Group* The osg::Group* corresponding to the dart::dynamics::BodyNode's collision shape
     */
    osg::Group* _makeBodyNodeCollisionMeshGroup(const dart::dynamics::BodyNode& node);

    /**
     * \brief Create an osgDart::BodyNodeVisuals* object from a dart::dynamics::BodyNode passed in
     * by reference. It generates the BodyNode specific visuals such as link frame and joint axis
     * for the passed-in BodyNode.
     * \param node const dart::dynamics::BodyNode reference for which to create visuals.
     * \return osgDart::BodyNodeVisuals pointer corresponding to the BodyNode's osg visuals
     */
    osgDart::BodyNodeVisuals* _makeBodyNodeVisuals(const dart::dynamics::BodyNode& node);

    /**
     * \brief Convert BodyNode visualization shapes to osg shapes
     * \param node BodyNode to get shapes from
     * \return void
     */
    void _addVisualizationShapesFromBodyNode(const dart::dynamics::BodyNode& node);

    /**
     * \brief Convert BodyNode collision shapes to osg shapes
     * \param node BodyNode to get the collision shapes from
     * \return void
     */
    void _addCollisionShapesFromBodyNode(const dart::dynamics::BodyNode& node);

    /**
     * \brief Update SkeletonNode recursively based on Skeleton transforms. This traverses through each
     * BodyNode and its child BodyNodes and updates corresponding osg::MatrixTransform based on their
     * current world transform.
     * \param bodyNode BodyNode used to update the osg::MatrixTransform and its children
     * \return void
     */
    void _updateRecursively(const dart::dynamics::BodyNode& bodyNode);

    /**
     * \brief Updates the skeleton visuals
     * \return void
     */
    void _updateSkeletonVisuals();

    //---------------------------------------------------------------
    //                    PROTECTED VARIABLES
    //---------------------------------------------------------------

    /// Root BodyNode
    const dart::dynamics::BodyNode& _rootBodyNode;

    /// Array of osg::Group pointers for the dart::dynamics::BodyNode visualization objects
    std::vector<osg::ref_ptr<osg::Group> > _bodyNodeGroups;

    /// Array of osgDart::BodyNodeVisuals ref pointers for showing/hiding them
    std::vector<osg::ref_ptr<osgDart::BodyNodeVisuals> > _bodyNodeVisuals;

    /// osgDart::SkeletonVisuals ref pointer for showing/hiding them
    osg::ref_ptr<osgDart::SkeletonVisuals> _skeletonVisuals;

    /// Array of osg::Group ref pointers for showing/hiding them
    std::vector<osg::ref_ptr<osg::Group> > _bodyNodeCollisionMeshGroups;

    /// Map from dart::dynamics::Joint* to osg::MatrixTransform*
    BodyNodeMatrixMap _bodyNodeMatrixMap;

    /// Map from dart::dynamics::BodyNode* to osg::Group* for the visualization shapes
    BodyNodeGroupMap _bodyNodeGroupMap;

    /// Map from dart::dynamics::BodyNode* to osg::Group* for the collision shapes
    BodyNodeGroupMap _bodyNodeCollsionMeshGroupMap;

    /// Map from dart::dynamics::BodyNode* to osgDart::BodyNodeVisuals for BodyNode visual shapes
    BodyNodeVisualsMap _bodyNodeVisualsMap;

    /// Debug variable for whether or not to print debug output
    const bool _debug;

}; // end class SkeletonNode

} // end osgDart namespace

#endif // SKELETONNODE_H
