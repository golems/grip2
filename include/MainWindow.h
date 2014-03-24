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

using namespace std;

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void Toolbar();
    void loadPlugins();

protected:
    QToolBar* getToolBar();

protected slots:
    void load();
    void quickLoad();
    void saveScene();
    void close();
    void exit();
    virtual void front() = 0;
    virtual void top() = 0;
    virtual void side() = 0;
    virtual void startSimulation() = 0;
    virtual void stopSimulation() = 0;
    virtual void simulateSingleStep() = 0;
    virtual void renderDuringSimulation() = 0;
    virtual void white() = 0;
    virtual void gray() = 0;
    virtual void black() = 0;
    virtual void resetCamera() = 0;
    virtual void xga1024x768() = 0;
    virtual void vga640x480() = 0;
    virtual void hd1280x720() = 0;
    virtual void simulationStopped() = 0;
    virtual void setSimulationRelativeTime(double time) = 0;
    void setMessageSlot(QString msg);
    void about();

private:
    void createActions();
    void createMenus();
    virtual void createRenderingWindow() = 0;
    virtual void createTreeView() = 0;
    virtual void createTabs() = 0;
    virtual void doLoad(string fileName) = 0;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *simulationMenu;
    QMenu *settingsMenu;
    QMenu *renderMenu;
    QMenu *helpMenu;
    QMenu *backgroundMenu;

    QToolBar* toolbar;
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
