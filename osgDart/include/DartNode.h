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
 * \file DartNode.h
 * \brief Class that is a subclass of osg::Group, which is the main
 * object for DART visualization in OpenSceneGraph. It can contain many
 * SkeletonNodes as parts of a simulation world. In essence, a DartNode is
 * the largest visualization object.
 */

#ifndef DARTNODE_H
#define DARTNODE_H

// DART includes
#include <dart/utils/urdf/DartLoader.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/simulation/World.h>

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>

#include "SkeletonNode.h"

using namespace dart;

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

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
    DartNode();

    /**
     * \brief Create a dart::dynamics::Skeleton pointer from a robot urdf file
     * using DART's DartLoader.
     * \param urdfFile The name of the urdf file
     * \return Pointer to the dynamics::Skeleton object
     */
    dynamics::Skeleton* parseRobotUrdf(std::string urdfFile);

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
    int addWorld(std::string urdfFile);

    /**
     * \brief Add a dart::dynamics::Skeleton to the DartNode using the name of
     * a robot urdf file.
     * \param urdfFile The name of the urdf file
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int addRobot(std::string urdfFile);

    /**
     * \brief Add a robot to the DartNode
     * \param robot New robot to add to the DartNode
     * \return Index (size_t) of the newly added robot
     */
    size_t addRobot(dynamics::Skeleton& robot);

    /**
     * \brief Add a world to the DartNode. The world may consist
     * of one or more objects or robots, all represented as Skeletons,
     * each with a world transformation.
     * \param world The dart::simulation::World object passed in by reference
     * that contains one or more dart::dynamics::Skeleton objects.
     * \return Index of the last object added
     */
    size_t addWorld(simulation::World& world);

    /**
     * \brief Get robot via index (size_t)
     * \param robotIndex Index of the robot you want
     * \return a dart::dynamics::Skeleton robot
     */
    dynamics::Skeleton* getRobot(size_t robotIndex);

    /**
     * \brief Print out meta information of the robot
     * \param robotIndex Index of the robot about which to print info
     * \return void
     */
    void printRobotInfo(size_t robotIndex);

    void update();
    
protected:

    //---------------------------------------------------------------
    //                       PROTECTED FUNCTIONS
    //---------------------------------------------------------------

    /**
     * \brief Helper function to determine if the index the user passed in to
     * specify a robot is valid.
     * \param robotIndex The index of the desired robot
     * \return A success/fail integer. 1 = Success. 0 = Fail.
     */
    int robotIndexIsValid(size_t robotIndex);


    //---------------------------------------------------------------
    //                       PROTECTED VARIABLES
    //---------------------------------------------------------------

    /// Standard vector of pointers to Skeletons
    std::vector<dynamics::Skeleton*> _robots;
    std::vector<SkeletonNode*> _skeletonNodes;

};

} // end namespace osgDart

#endif // DARTNODE_H
