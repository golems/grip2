/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Sungmoon Joo <sungmoon.joo@gmail.com>
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
 * \file InspectorTab.h
 * \brief Class that handles all primitive interaction affecting orientation
 * and position of objects in the simulation world
 */

#ifndef INSPECTOR_TAB_H
#define INSPECTOR_TAB_H

// Local includes
#include "TreeView.h"

// QT includes
#include <QDoubleSpinBox>

// DART includes
#include <dart/simulation/World.h>


namespace Ui { class InspectorTab; }


/**
 * \class InspectorTab InspectorTab.h
 * \brief Widget containing all the widgets that allow the user to manually manipulate
 * a skeleton. Subclasses QDockWidget.
 */
class InspectorTab : public QDockWidget {

    /// meta object macro for signals and slots usage
    Q_OBJECT

public:
    /**
     * \brief Constructs an InspectorTab object
     * \param parent Object constructing this InspectorTab object
     * \param simWorld Pointer to the simulation world
     * \param treeViewer Pointer to the TreeView
     */
    InspectorTab(QWidget *parent, dart::simulation::World *simWorld, TreeView *treeViewer);

    /**
     * \brief Destructs an InspectorTab object
     */
    ~InspectorTab();

    /**
     * \brief Initializes the tab widget
     * \return void
     */
    void initializeTab();


private slots:
    void changeSelectedJoint(int sliderValue); //, simulation::World* mWorld, Tree_View* treeviewer);
    void receiveSeletedItem(TreeViewReturn *active_item);
    void changePositionAndOrientation(int sliderValue);


private:
    /// Pointer to all the widgets of the UI
    Ui::InspectorTab *_ui;
    /// Pointer to the simulation world object
    dart::simulation::World *_simWorld;
    /// Pointer to the TreeView object
    TreeView *_treeview;
    /// Pointer to the position slider widget
    QSlider *_positionSlider_0;
    /// Pointer to the position spin box widget
    QDoubleSpinBox *_positionSpinBox_0;
    /// Type of the currently selected TreeView item
    int _selectedTypeFromTree;

    /**
     * \brief Gets the root transform of a Skeleton pointer passed in
     * \param skel Skeleton pointer of which to get the root transform
     * \return Eigen::Matrix6d containing the root transform of the skeleton passed in
     */
    Eigen::Matrix<double, 6, 1> getRootTransform(dart::dynamics::Skeleton *robot);

    /**
     * \brief Sets the root transform of a Skeleton pointer passed in
     * \param skel Pointer to the skeleton of which to change the root transform
     * \param pose New transformation of the skeletons root
     * \return void
     */
    void setRootTransform(dart::dynamics::Skeleton *robot, const Eigen::Matrix<double, 6, 1> &pose);

    /**
     * \brief Converts a transform to a xyz roll, pitch, yaw vector
     * \param tf Transformation to convert
     * \return Vector containing the xyz and roll, pitch, yaw values of the transform
     */
    Eigen::Matrix<double, 6, 1> getPoseFromTransform(const Eigen::Isometry3d &tf);

}; // end class InspectorTab

#endif // INSPECTOR_TAB_H

