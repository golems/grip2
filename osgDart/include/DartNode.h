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
 * \brief Class that is a subclass of osg::Group, which is the main\n
 * object for DART visualization in OpenSceneGraph
 */

#ifndef DARTNODE_H
#define DARTNODE_H

// DART includes
#include <dart/utils/urdf/DartLoader.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/BodyNode.h>

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/Matrix>
#include <osg/MatrixTransform>

using namespace dart;

namespace osgDart {

/**
 * \class DartNode DartNode.h
 * \brief Class that is a subclass of osg::Group, which is the main
 * object for DART visualization in OpenSceneGraph
 */
class DartNode : public osg::Group
{
public:

    /**
     * \brief Constructor for DartNode
     */
    DartNode();

    dynamics::Skeleton* getRobot(std::string urdfFile);
    size_t addRobot(std::string urdfFile);

    /**
     * \brief Add a robot to the DartNode
     * \param robot New robot to add to the DartNode
     * \return Index (size_t) of the newly added robot
     */
    size_t addRobot(dynamics::Skeleton& robot);

    /**
     * \brief Get robot via index (size_t)
     * \param robotIndex Index of the robot you want
     * \return a dart::dynamics::Skeleton robot
     */
    dynamics::Skeleton* getRobot(size_t robotIndex);

    void printRobotInfo(size_t robotIndex);
    
protected:

    /// Standard vector of pointers to Skeletons
    std::vector<dynamics::Skeleton*> _robots;

    bool robotIndexIsValid(size_t robotIndex);
};

} // end namespace osgDart

#endif // DARTNODE_H
