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

#ifndef GRIP_CAMERA_MANIPULATOR_H
#define GRIP_CAMERA_MANIPULATOR_H

// OpenSceneGraph includes
#include <osgGA/OrbitManipulator>

/**
 * \namespace osgGolems
 * \brief Namespace for all the classes that are only dependent upon OpenSceneGraph
 */
namespace osgGolems {

/**
 * \class CameraManipulator CameraManipulator.h
 * \brief Camera manipulator class that subclasses osgGA::OrbitManipulator.
 * It sets up desired behavior for robot simulator.
 */
class GRIPCameraManipulator : public osgGA::OrbitManipulator
{
public:

    /**
     * \brief Constructor for GRIPCameraManipulator class
     */
    GRIPCameraManipulator();

    /**
     * \brief Destructor for GRIPCameraManipulator class
     */
    ~GRIPCameraManipulator();

    /**
     * \brief Set the camera manipulator's center of rotation
     * \param center osg::Vec3 specifying x,y,z coordinates of center of rotation
     * \return void
     */
    void setCenter(osg::Vec3 center);

protected:

    /**
     * \brief keyboard and mouse event handler
     */
    virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
    /**
     * \brief keyboard and mouse event handler
     */
    virtual bool handleKeyUp( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
    /**
     * \brief keyboard and mouse event handler
     */
    virtual bool handleMouseMove( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
    /**
     * \brief a boolean member variable to trace whether a keyboard is pressed
     */
    bool _keyPressed; //true = pressed, false = released
    /**
     * \brief a member variable to trace the pressed key
     */
    int _pressedKey;

    float _currentX;
    float _previousX;
    float _currentY;
    float _previousY;

    bool _keyboardMouseBinding; // true - when keyboard and mouse are used for camera control

}; // end class CameraManipulator

} // end namespace osgGolems

#endif // GRIP_CAMERA_MANIPULATOR_H
