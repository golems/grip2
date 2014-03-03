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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <cstdio>
#include <ViewerWidget.h>
#include <tree_view.h>
#include "ui_visualizer.h"
#include "ui_inspector.h"
#include "ui_tree_view.h"
#include "DartNode.h"

using namespace std;

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow , private Ui::Visualizer, private Ui::Inspector
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void Toolbar();
    ViewerWidget* viewWidget;
    Tree_View* treeviewer;
    Visualizer *visualizertab;
    Inspector  *inspectortab;
    osgDart::DartNode* dartNode;
    osgDart::DartNode* worldNode;
    osg::Matrixd frontView, sideView, topView;

private slots:
    void load();
    void quickLoad();
    void saveScene();
    void close();
    void exit();
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
    void about();

private:
    void createActions();
    void createMenus();
    void createOsgWindow();
    void createTreeView();
    void createTabs();
    void doLoad(string fileName);
    int saveText(string scenepath, const char* llfile);

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *simulationMenu;
    QMenu *settingsMenu;
    QMenu *renderMenu;
    QMenu *helpMenu;
    QMenu *backgroundMenu;
    QActionGroup *colorGroup;
    QAction *loadAct;
    QAction *quickLoadAct;
    QAction *saveSceneAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *frontAct;
    QAction *topAct;
    QAction *sideAct;
    QAction *startSimulationAct;
    QAction *stopSimulationAct;
    QAction *simulateSingleStepAct;
    QAction *renderDuringSimulationAct;
    QAction *whiteAct;
    QAction *grayAct;
    QAction *blackAct;
    QAction *resetCameraAct;
    QAction *xga1024x768Act;
    QAction *vga640x480Act;
    QAction *hd1280x720Act;
    QAction *aboutAct;
};

#endif // MAINWINDOW_H
