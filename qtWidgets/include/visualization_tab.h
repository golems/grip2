/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael X. Grey <mxgrey@gatech.edu>
 * Date: Jan 2014
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
 * \file Visualization_Tab.h
 * \brief Widget containing all the visualization settings widgets all
 * contained in a DockableWidget
 */

#ifndef VISUALIZATION_TAB_H
#define VISUALIZATION_TAB_H

// Local includes
#include "ui_visualization_tab.h"
#include "DartNode.h"
#include "TreeView.h"

/**
 * \class Visualization_Tab Visualization_Tab.h
 * \brief Widget containing all the visualization settings widgets all
 * contained in a DockableWidget
 */
class Visualization_Tab : public QDockWidget {

    /// Q_Object macro for using signals and slots
    Q_OBJECT

public:
    /**
     * \brief Constructs a Visualization_Tab object
     */
    Visualization_Tab(osgDart::DartNode* worldNode, TreeView* treeView, QWidget *parent = 0);

    /**
     * \brief Destructs a Visualization_Tab object
     */
    ~Visualization_Tab();

    /**
     * \brief Updates the world based on the current visualization settings.
     * This is useful for having newly loaded objects render with the current
     * visualization settings
     * \return void
     */
    void update();

public slots:

    /**
     * \brief Shows/hides the joint axes of the skeletons
     * \param checked Whether or not to show the axes
     * \return void
     */
    void slotToggleJointAxesVisibility(bool checked);

    /**
     * \brief Shows/hides the link frames of the skeletons
     * \param checked Whether or not to show the frames
     * \return void
     */
    void slotToggleBodyNodeAxesVisibility(bool checked);

    /**
     * \brief Shows/hides the center of mass of the skeletons
     * \param checked Whether or not to show the center of masses
     * \return void
     */
    void slotToggleSkeletonCoMVisibility(bool checked);

    /**
     * \brief Shows/hides the projected center of mass of the skeletons
     * \param checked Whether or not to show the projected center of masses
     * \return void
     */
    void slotToggleSkeletonProjCoMVisibility(bool checked);

    /**
     * \brief Turns on/off wireframe mode for all the skeletons
     * \param checked Whether or not to render using wireframe mode
     * \return void
     */
    void slotToggleSkeletonWireFrameMode(bool checked);

    /**
     * \brief Turns on/off collision mesh render mode for all the skeletons
     * \param checked Whether or not to render using the collision mesh instead
     * of the visualization mesh
     * \return void
     */
    void slotToggleSkeletonCollisionMeshMode(bool checked);

    /**
     * \brief Sets the transparency level of a skeleton or BodyNode
     * \param transparencyValue Current transparency value of the slider
     * \return void
     */
     void slotSetTransparencyValue(int transparencyValue);

protected:
    /// Ui object that contains all the widgets of the tab
    Ui::Visualization_Tab *visualizer_ui;

    /// osgDart::DartNode pointer in order to change visualization options of the the world
    osgDart::DartNode* _worldNode;

    /// Tree_View pointer for getting the current object
    TreeView* _treeView;

}; // end class Visualization_Tab

#endif // VISUALIZATION_TAB_H
