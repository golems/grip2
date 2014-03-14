/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Shailesh Lohia <shailesh.lohia@gatech.edu>
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
#ifndef GRIPMAINWINDOW_H
#define GRIPMAINWINDOW_H


#include <iostream>
#include <cstdio>
#include <QList>

///including the base class
#include "mainwindow.h"

#include <ViewerWidget.h>
#include <tree_view.h>

#include <inspector_tab.h>
#include <visualization_tab.h>
#include "ui_visualization_tab.h"
#include "ui_inspector_tab.h"

#include "ui_tree_view.h"
#include "GripTab.h"

#include "DartNode.h"
#include "GripSimulation.h"

using namespace std;

class GripMainWindow : public MainWindow
{
public:
    GripMainWindow();
    ~GripMainWindow();

    /// OpenSceneGraph Qt composite viewer widget, which can hold more than one view
    ViewerWidget* viewWidget;

    Inspector_Tab* inspectortab;
    Visualization_Tab* visualizationtab;

    /// TreeViewReturn class is defined in tree_view.h
    /// It contains two members: void* object and DataType dataType
    /// void* object can store a dart Skeleton object or BodyNode object
    /// DataType is an enumaration defined in tree_view.h which can take
    /// the values Return_Type_Robot and Return_Type_Node
    TreeViewReturn* activeItem;

    /// Main OpenSceneGraph node for the main view of the composite viewer
    osgDart::DartNode* worldNode;
    dart::simulation::World* world;

    GripSimulation* simulation;

    dart::dynamics::Skeleton* createGround();

protected slots:
    void setSimulationRelativeTime(double time);

private slots:
    void front();
    void top();
    void side();
    void startSimulation();
    void stopSimulation();
    void simulateSingleStep();
    void renderDuringSimulation();
    void white();
    void gray();
    void black();
    void resetCamera();
    void xga1024x768();
    void vga640x480();
    void hd1280x720();
    void simulationStopped();

private:
    /** Private Members */
    /// Any plugin that is loaded successfully into the Grip will get stored in this QList
    /// The plugins are always going to be derived from the GripTab interface defined in qtWidgets/include/GripTab.h
    /// In order to execute one of the pure virtual functions defined in the GripTab declaration,
    /// you first need to perform a qobject_cast. An example for the same is
    /// GripTab* gt = qobject_cast<GripTab*>(plugin);
    /// Once a pointer of type GripTab is created you can then call the function directly.
    QList<GripTab*>* pluginList;

    /// QDockWidget that contains a QTreeWidget. It is used as an object explorer for the loaded skeletons or robots
    Tree_View* treeviewer;

    /** Private Methods */
    void createRenderingWindow();
    void createTreeView();
    void createTabs();
    void clear();
    bool stopSimulationWithDialog();
    void swapStartStopButtons();
    void doLoad(string fileName);
    int saveText(string scenepath, const char* llfile);

    bool _simulating;

    /// This function reads a folder by the name of 'plugin' in source directory
    /// The plugin directory needs to contain the library for the plugins to be loaded (.so files on Linux, .dll files on Windows)
    void loadPlugins();
};

#endif // GRIPMAINWINDOW_H
