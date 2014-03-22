/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
 *
 * Humanoid skeletonics Lab      Georgia Institute of Technology
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
 * \file DartNode.h
 * \brief Class that is a subclass of osg::Group, which is the main
 * object for DART visualization in OpenSceneGraph. It can contain many
 * SkeletonNodes as parts of a simulation world. In essence, a DartNode is
 * the largest visualization object.
 */

#ifndef DARTNODE_H
#define DARTNODE_H

// DART includes
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/simulation/World.h>

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>

// osgDart includes
#include "SkeletonNode.h"

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

/// Definition of type SkeletonNodeMap, which maps dart::dynamics::Skeleton* to SkeletonNode*
typedef std::map<const dart::dynamics::Skeleton*, osg::ref_ptr<SkeletonNode> > SkeletonNodeMap;


/**
 * \class DartNode DartNode.h
 * \brief Class that is a subclass of osg::Group, which is the main
 * object for DART visualization in OpenSceneGraph. It can contain many
 * SkeletonNodes as parts of a simulation world. In essence, a DartNode is
 * the largest visualization object.
 */
class DartNode : public osg::Group
{
public:

    //---------------------------------------------------------------
    //                       PUBLIC FUNCTIONS
    //---------------------------------------------------------------

    /**
     * \brief Constructor for DartNode
     */
    DartNode(bool debug=false);

    /**
     * \brief Create a dart::dynamics::Skeleton pointer from a skeleton urdf file
     * using DART's DartLoader.
     * \param urdfFile The name of the urdf file
     * \return Pointer to the dynamics::Skeleton object
     */
    dart::dynamics::Skeleton* parseSkeletonUrdf(std::string urdfFile);

    /**
     * \brief Create a dart::simulation::World pointer from a world sdf file
     * using DART's Sdf parser.
     * \param sdfFile The name of the world sdf file
     * \return Pointer to the simulation::World object
     */
    dart::simulation::World* parseWorldSdf(std::string sdfFile);

    /**
     * \brief Create a dart::simulation::World pointer from a world urdf file
     * using DART's DartLoader.
     * \param urdfFile The name of the urdf file
     * \return Pointer to the simulation::World object
     */
    dart::simulation::World* parseWorldUrdf(std::string urdfFile);

    /**
     * \brief Add a dart::simulation::World to the DartNode using the name of
     * a world urdf file.
     * \param urdfFile The name of the urdf file
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int addWorldFromUrdf(std::string urdfFile);

    /**
     * \brief Add a dart::simulation::World to the DartNode using the name of
     * a world sdf file.
     * \param sdfFile The name of the sdf file
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    size_t addWorldFromSdf(std::string sdfFile);

    size_t addWorld(std::string file);

    /**
     * \brief Add a dart::dynamics::Skeleton to the DartNode using the name of
     * a skeleton urdf file.
     * \param urdfFile The name of the urdf file
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    size_t addSkeleton(std::string urdfFile);

    /**
     * \brief Add a skeleton to the DartNode
     * \param skeleton New skeleton to add to the DartNode
     * \return Index (size_t) of the newly added skeleton
     */
    size_t addSkeleton(dart::dynamics::Skeleton &skeleton);

    /**
     * \brief Add a world to the DartNode. The world may consist
     * of one or more objects or skeletons, all represented as Skeletons,
     * each with a world transformation.
     * \param world The dart::simulation::World object passed in by reference
     * that contains one or more dart::dynamics::Skeleton objects.
     * \return Index of the last object added
     */
    size_t addWorld(dart::simulation::World* world);

    /**
     * \brief Get skeleton via index (size_t)
     * \param skeletonIndex Index of the skeleton you want
     * \return a dart::dynamics::Skeleton skeleton
     */
    dart::dynamics::Skeleton* getSkeleton(size_t skeletonIndex=0);

    /**
     * \brief Remove skeleton from DartNode by passing in the pointer to
     * the dart::dynamics::Skeleton to be removed.
     * \param skeleton skeleton to remove from the DartNode
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int removeSkeleton(const dart::dynamics::Skeleton* skeletonToRemove);

    /**
     * \brief Remove skeleton from DartNode by passing in the index of the skeleton
     * to be removed.
     * \param skeleton skeleton to remove from the DartNode
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int removeSkeleton(size_t skeletonIndex=0);

    /**
     * \brief Clear the DartNode. This is used for when you want to start from scratch but don't
     * want to delete and create a new DartNode.
     * \return void
     */
    void clear();

    /**
     * \brief Get a pointer to the World object in the DartNode.
     * \return simulation::World pointer
     */
    dart::simulation::World* getWorld();

    /**
     * \brief Get number of skeletons in the DartNode
     * \return size_t Number of skeletons in the DartNode
     */
    size_t getNumSkeletons();

    /**
     * \brief Print out meta information of the skeleton
     * \param skeletonIndex Index of the skeleton about which to print info
     * \return void
     */
    void printInfo();

    /**
     * \brief Updates the transforms of all the dart objects in the SkeletonNodes
     * of the DartNode for the next culling and drawing events.
     * \return void
     */
    void update();

    /**
     * \brief Sets the transparency value of the SkeletonNode corresponding to the
     * dart::dynamics::Skeleton passed in
     * \param skel Skeleton of which to change the transparency value of
     * \param transparencyValue New transparency value for the skeleton
     * \return void
     */
    void setSkeletonTransparency(const dart::dynamics::Skeleton& skel, float transparencyValue);

    /**
     * \brief Sets the transparency value of the osg::Group corresponding to the
     * dart::dynamics::BodyNode passed in
     * \param node BodyNode of which to change the transparency value of
     * \param transparencyValue New transparency value for the BodyNode
     * \return void
     */
    void setBodyNodeTransparency(const dart::dynamics::BodyNode& node, float transparencyValue);

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
     * \brief Render the skeleton using the collision mesh instead of the visual mesh
     * \param enable Whether or not to enable the collision mesh
     * \return void
     */
    void setSkeletonCollisionMeshOn(bool enable=true);

    /**
     * \brief Render the skeleton using wireframe instead of fill mode.
     * \param enable Whether or not to enable the wireframe mode
     * \return void
     */
    void setSkeletonWireFrameOn(bool enable=true);

    /**
     * \brief Hide a skeleton
     * \param int skeleton index
     * \return void
     */
    void hideSkeleton(int i);

protected:

    //---------------------------------------------------------------
    //                       PROTECTED FUNCTIONS
    //---------------------------------------------------------------

    /**
     * \brief Helper function to determine if the index the user passed in to
     * specify a skeleton is valid.
     * \param skeletonIndex The index of the desired skeleton
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int skeletonIndexIsValid(size_t skeletonIndex);


    //---------------------------------------------------------------
    //                       PROTECTED VARIABLES
    //---------------------------------------------------------------

    /// Standard vector of pointers to Skeletons
    dart::simulation::World* _world;

    /// Standard vector of pointers to dart::dynamics::Skeleton objects
    std::vector<dart::dynamics::Skeleton*> _skeletons;

    /// Standard vector of pointers to SkeletonNode objects
    std::vector<osg::ref_ptr<SkeletonNode> > _skeletonNodes;

    /// Map from dart::dynamics::Skeleton* to osg::SkeletonNode
    SkeletonNodeMap _skelNodeMap;

    /// Debug variable for whether or not to print debug output
    const bool _debug;
};

} // end namespace osgDart

#endif // DARTNODE_H
