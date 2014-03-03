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
 * \file CameraManipulator.h
 * \brief Camera manipulator class that subclasses osgGA::OrbitManipulator.
 * It sets up desired behavior for robot simulator.
 */

#ifndef CAMERA_MANIPULATOR_H
#define CAMERA_MANIPULATOR_H

// OpenSceneGraph includes
#include <osgGA/OrbitManipulator>

/**
 * \class CameraManipulator CameraManipulator.h
 * \brief Camera manipulator class that subclasses osgGA::OrbitManipulator.
 * It sets up desired behavior for robot simulator.
 */
class CameraManipulator : public osgGA::OrbitManipulator
{
public:

    /**
     * \brief Constructor for CameraManipulator class
     */
    CameraManipulator();

    /**
     * \brief Destructor for CameraManipulator class
     */
    ~CameraManipulator();

    /**
     * \brief Set the camera manipulator's center of rotation
     * \param center osg::Vec3 specifying x,y,z coordinates of center of rotation
     * \return void
     */
    void setCenter(osg::Vec3 center);

protected:



};

#endif // CAMERA_MANIPULATOR_H