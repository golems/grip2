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
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::AutomaticSelection);

    /**
     * \brief Add a osgQt::GraphicsWindowQt widget to the ViewerWidget
     * \param camera An osg::Camera pointer
     * \param scene An osg::Node pointer which is the scene to display
     * \return QWidget pointer
     */
    QWidget* addViewWidget(osg::Camera* camera, osg::Node* scene);

    /**
     * \brief Creates a camera for the viewer
     * \param x x-coordinate of the upper-left corner of the window
     * \param y y-coordinate of the upper-left corner of the window
     * \param w Width of the window
     * \param h Height of the window
     * \param name Name of the window
     * \param windowDecoration Whether or not to add window decoration
     * \return void
     */
    osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    /**
     * \brief Gets the view matrix
     * \return osg::Matrixd
     */
    osg::Matrixd getViewMatrix();

    /**
     * \brief Sets the viewMatrix of the specified view
     * \param i View to change the view matrix of
     * \param m New view matrix
     * \return void
     */
    void setViewMatrix(uint i, osg::Matrixd m);

    /**
     * \brief Sets the matrix of the camera manipulator to front view
     * \return void
     */
    void setToFrontView();

    /**
     * \brief Sets the matrix of the camera manipulator to top view
     * \return void
     */
    void setToTopView();

    /**
     * \brief Sets the matrix of the camera manipulator to side view
     * \return void
     */
    void setToSideView();

    /**
     * \brief Sets the matrix of the specified camera
     * \param newMatrix osg::Matrix reference to set the camera to
     * \param viewNum The view of which to set the camera matrix
     * \return void
     */
    void setCameraMatrix(osg::Matrix& newMatrix, uint viewNum=0);

    /**
     * \brief Adds an osg node to the scene in the view specified by the user
     * \param node osg::Node pointer to add to the scene
     * \param viewNum View of which to add the node to
     * \return void
     */
    void addNodeToScene(osg::Node *node, uint viewNum=0);

    /**
     * \brief Sets background color of the view number "viewNum" with the
     * "color". If an invalid view number is provided an error message is printed
     * and no change occurs to any view.
     * \param color osg::Vec4 RGBA (red, green blue, alpha) color vector.
     * \param viewNum View number to change the background color of.
     * \return void
     */
    void setBackgroundColor(const osg::Vec4& color, uint viewNum=0);

    /**
     * \brief Gets the camera manipulator for a certain view, specified
     * by it's view number "viewNum".
     * \param viewNum Number of the view of which to get the camera manipulator.
     * \return osgGA::CameraManipulator pointer.
     */
    osgGA::CameraManipulator* getCameraManipulator(uint viewNum=0);

    /**
     * \brief Sets the the camera the in the specified view to its home position
     * \param viewNum View for which to set the camera to home position
     * \return void
     */
    void setCameraToHomePosition(uint viewNum=0);

    /**
     * \brief Adds a grid to the viewer
     * \param width Width of the entire grid in meters
     * \param depth Depth of the entire grid in meters
     * \param gridSize Length of side of each grid square in meters
     * \return void
     */
    void addGrid(uint width, uint depth, uint gridSize);

    /**
     * \brief Renders the scene
     * \param event QPaint event
     * \return void
     */
    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

    /**
     * \brief takes a screenshot of the view widget
     * \return void
     */
    QImage takeScreenshot();

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
}; // end class ViewerWidget

#endif // VIEWER_WIDGET_H
