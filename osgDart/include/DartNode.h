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

using namespace dart;

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

/// Definition of type SkeletonNodeMap, which maps dart::dynamics::Skeleton* to SkeletonNode*
typedef std::map<dynamics::Skeleton*, osg::ref_ptr<SkeletonNode> > SkeletonNodeMap;


/**
 * \class DartNode DartNode.h
 * \brief Class that is a subclass of osg::Group, which is the main
 * object for DART visualization in OpenSceneGraph. It can contain many
 * SkeletonNodes as parts of a simulation world. In essence, a DartNode is
 * the largest visualization object.
 */
class DartNode : public osg::Switch
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
    dynamics::Skeleton* parseSkeletonUrdf(std::string urdfFile);

    /**
     * \brief Create a dart::simulation::World pointer from a world sdf file
     * using DART's Sdf parser.
     * \param sdfFile The name of the world sdf file
     * \return Pointer to the simulation::World object
     */
    simulation::World* parseWorldSdf(std::string sdfFile);

    /**
     * \brief Create a dart::simulation::World pointer from a world urdf file
     * using DART's DartLoader.
     * \param urdfFile The name of the urdf file
     * \return Pointer to the simulation::World object
     */
    simulation::World* parseWorldUrdf(std::string urdfFile);

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
    size_t addSkeleton(dynamics::Skeleton* skeleton);

    /**
     * \brief Add a world to the DartNode. The world may consist
     * of one or more objects or skeletons, all represented as Skeletons,
     * each with a world transformation.
     * \param world The dart::simulation::World object passed in by reference
     * that contains one or more dart::dynamics::Skeleton objects.
     * \return Index of the last object added
     */
    size_t addWorld(simulation::World *world);

    /**
     * \brief Get skeleton via index (size_t)
     * \param skeletonIndex Index of the skeleton you want
     * \return a dart::dynamics::Skeleton skeleton
     */
    dynamics::Skeleton* getSkeleton(size_t skeletonIndex=0);

    /**
     * \brief Remove skeleton from DartNode by passing in the pointer to
     * the dart::dynamics::Skeleton to be removed.
     * \param skeleton skeleton to remove from the DartNode
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int removeSkeleton(dart::dynamics::Skeleton* skeletonToRemove);

    /**
     * \brief Remove skeleton from DartNode by passing in the index of the skeleton
     * to be removed.
     * \param skeleton skeleton to remove from the DartNode
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int removeSkeleton(size_t skeletonIndex=0);

    void removeAllSkeletons();

    /**
     * \brief Get a pointer to the World object in the DartNode.
     * \return simulation::World pointer
     */
    simulation::World* getWorld();

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
     * \brief Update the transforms of all the dart objects in the SkeletonNodes
     * of the DartNode for the next culling and drawing events.
     * \return void
     */
    void update();

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
    simulation::World* _world;

    /// Standard vector of pointers to dart::dynamics::Skeleton objects
    std::vector<dynamics::Skeleton*> _skeletons;

    /// Standard vector of pointers to SkeletonNode objects
    std::vector<osg::ref_ptr<SkeletonNode> > _skeletonNodes;

    SkeletonNodeMap _skelNodeMap;

    bool _debug;

};

} // end namespace osgDart

#endif // DARTNODE_H
