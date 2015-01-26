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
 * \file CameraCallback.h
 * \brief Class for Camera callback
 */

#ifndef CAMERA_CALLBACK_H
#define CAMERA_CALLBACK_H

// OpenSceneGraph includes
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osgViewer/View>
#include <osg/io_utils>

// C++ Standard includes
#include <iostream>

/**
 * \class CameraData CameraCallback.h
 * \brief Data for the camera callback
 */
class CameraData : public osg::Referenced
{
public:
    /// TF of the camera
    osg::MatrixTransform* tf;
    /// View that the camera belongs to
    osgViewer::View* view;
};

/**
 * \class CameraCallback CameraCallback.h
 * \brief Class for the camera callback
 */
class CameraCallback : public osg::NodeCallback
{
public:
    /**
     * \brief Constructs a CameraCallback object
     */
    CameraCallback()
    {

    }

    /**
     * \brief Callback operator
     */
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::ref_ptr<CameraData> data =
                dynamic_cast<CameraData*> (node->getUserData());
        if(data) {
            osg::Matrix m = data->view->getCamera()->getInverseViewMatrix();
            data->tf->setMatrix(m);
        }

        traverse(node, nv);
    }

}; // end class CameraCallback

#endif // CAMERA_CALLBACK_H
