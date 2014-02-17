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
 * \file ViewerWidget.h
 * \brief Class that is an osgViewer::CompositeViewer as a QWidget
 */

#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

// QT includes
#include <QtCore/QTimer>
#include <QtGui/QGridLayout>

// OpenSceneGraph includes
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include <osg/io_utils>
#include "osgUtils.h"

// Standard Library includes
#include <iostream>

/**
 * \class ViewerWidget ViewerWidget.h
 * \brief Class which inherits from QWidget and osgViewer::CompositeViewer.
 * This class creates a osgQt::GraphicsWindowQt and puts it inside a composite viewer
 */
class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{

public:
    /**
     * \brief Constructor for ViewerWidget class
     */
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded);

    /**
     * \brief Add a osgQt::GraphicsWindowQt widget to the ViewerWidget
     * \param camera An osg::Camera pointer
     * \param scene An osg::Node pointer which is the scene to display
     * \return QWidget pointer
     */
    QWidget* addViewWidget(osg::Camera* camera, osg::Node* scene);

    osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    osg::Matrixd getViewMatrix();
    void setViewMatrix(uint i, osg::Matrixd m);

    void addNodeToScene(osg::Node *node, uint viewNum=0);

    /**
     * \brief Set background color of the view number "viewNum" with the
     * "color". If an invalid view number is provided an error message is printed
     * and no change occurs to any view.
     * \param color osg::Vec4 RGBA (red, green blue, alpha) color vector.
     * \param viewNum View number to change the background color of.
     * \return void
     */
    void setBackgroundColor(const osg::Vec4& color, uint viewNum=0);

    /**
     * \brief Get the camera manipulator for a certain view, specified
     * by it's view number "viewNum".
     * \param viewNum Number of the view of which to get the camera manipulator.
     * \return osgGA::CameraManipulator pointer.
     */
    osgGA::CameraManipulator* getCameraManipulator(uint viewNum=0);

    void setCameraToHomePosition(uint viewNum=0);

//    void addCenterAxes();

    osg::Node* getSceneData();

    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:

    // Timer for update the interface
    QTimer _timer;

    /**
     * \brief Determines if the input view number is valid,
     * i.e., Does that view exist in the ViewWidget, since the
     * ViewWidget is a CompositeViewer and therefore can hold more
     * than one view.
     * \param viewNum Number of the view to validate
     * \return bool Whether or not the view number exists
     */
    bool viewNumIsValid(uint viewNum);
};


#endif // VIEWER_WIDGET_H
