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


#ifndef INSPECTOR_H
#define INSPECTOR_H

// Local includes
#include "ui_inspector_tab.h"
#include "tree_view.h"

// DART includes
#include <dart/simulation/World.h>

/**
 * \class InspectorTab InspectorTab.h
 * \brief Widget containing all the widgets that allow the user to manually manipulate
 * a skeleton. Subclasses QDockWidget.
 */
class Inspector_Tab : public QDockWidget {

    /// meta object macro for signals and slots usage
    Q_OBJECT

public:
    /**
     * \brief Constructs an InspectorTab object
     */
    Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer);

    /**
     * \brief Destructs an InspectorTab object
     */
    ~Inspector_Tab();

private slots:
    void ChangePos0DoubleSpinBox(int sliderValue);
    void ChangePos0Slider(double spinBoxValue);
    void ChangeJoint(int sliderValue); //, simulation::World* mWorld, Tree_View* treeviewer);
    void ReceiveSeletedItem(TreeViewReturn* active_item);

private:
    Ui::Inspector_Tab *inspector_ui;
    simulation::World *simworld;
    Tree_View *treeview;
    QSlider *positionSlider_0;
    QDoubleSpinBox *positionSpinBox_0;
};

#endif

