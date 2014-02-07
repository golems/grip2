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

#include <QtGui>
#include "mainwindow.h"
#include "tree_view.h"
#include "viewer_widget.h"
//including the icons for the toolbar
#include "icons/open.xpm"
#include "icons/redo.xpm"
#include "icons/simulate.xpm"
#include "icons/stop.xpm"
#include "icons/camera.xpm"
#include "icons/film.xpm"
#include "icons/frontView.xpm"
#include "icons/topView.xpm"
#include "icons/rightSideView.xpm"


MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createOsgWindow();
    createTreeView();


    setWindowTitle(tr("Grip2"));
    //setMinimumSize(860, 640);
    resize(860, 640);
}

void MainWindow::Toolbar()
{
    QPixmap openIcon((const char**) open_xpm);
    QPixmap redoIcon((const char**) redo_xpm);
    QPixmap simulateIcon((const char**) simulate_xpm);
    QPixmap stopIcon((const char**) stop_xpm);
    QPixmap cameraIcon((const char**) camera_xpm);
    QPixmap filmIcon((const char**) film_xpm);
    QPixmap frontViewIcon((const char**) frontView_xpm);
    QPixmap topViewIcon((const char**) topView_xpm);
    QPixmap rightSideViewIcon((const char**) rightSideView_xpm);

    QToolBar *toolbar = addToolBar("main toolbar");
    QAction *open = toolbar->addAction(QIcon(openIcon), "Open Scene (Ctrl + O)");
    QAction *redo = toolbar->addAction(QIcon(redoIcon), "Load last viewed scene (Ctrl + Shift + Q)");
    toolbar->addSeparator();
    QAction *simulate = toolbar->addAction(QIcon(simulateIcon), "Start Simulation");
    QAction *stop = toolbar->addAction(QIcon(stopIcon), "Stop Simulation");
    toolbar->addSeparator();
    QAction *camera = toolbar->addAction(QIcon(cameraIcon), "Export screenshot");
    QAction *film = toolbar->addAction(QIcon(filmIcon), "Export film sequence");
    toolbar->addSeparator();
    QAction *front = toolbar->addAction(QIcon(frontViewIcon), "View scene from front");
    QAction *top = toolbar->addAction(QIcon(topViewIcon), "View scene from top");
    QAction *rightSide = toolbar->addAction(QIcon(rightSideViewIcon), "View scene from right");

    connect(open, SIGNAL(triggered()), this, SLOT(load()));
    connect(redo, SIGNAL(triggered()), this, SLOT(quickLoad()));
    connect(simulate, SIGNAL(triggered()), this, SLOT(startSimulation()));
    connect(stop, SIGNAL(triggered()), this, SLOT(stopSimulation()));
    connect(camera, SIGNAL(triggered()), this, SLOT(load()));
    connect(film, SIGNAL(triggered()), this, SLOT(load()));
    connect(front, SIGNAL(triggered()), this, SLOT(front()));
    connect(top, SIGNAL(triggered()), this, SLOT(top()));
    connect(rightSide, SIGNAL(triggered()), this, SLOT(side()));
}


void MainWindow::load(){}
void MainWindow::quickLoad(){}
void MainWindow::saveScene(){}
void MainWindow::close(){}
void MainWindow::exit(){}
void MainWindow::front(){}
void MainWindow::top(){}
void MainWindow::side(){}
void MainWindow::startSimulation(){}
void MainWindow::stopSimulation(){}
void MainWindow::simulateSingleStep(){}
void MainWindow::renderDuringSimulation(){}
void MainWindow::white(){}
void MainWindow::black(){}
void MainWindow::resetCamera(){}
void MainWindow::xga1024x768(){}
void MainWindow::vga640x480(){}
void MainWindow::hd1280x720(){}
void MainWindow::about(){}

void MainWindow::createActions()
{
    //loadAct
    loadAct = new QAction(tr("&Load"), this);
    loadAct->setShortcut(Qt::CTRL + Qt::Key_O);
    loadAct->setStatusTip(tr("Load scene"));
    connect(loadAct, SIGNAL(triggered()), this, SLOT(load()));


    //quickLoadAct
    quickLoadAct = new QAction(tr("&Quick Load"), this);
    quickLoadAct->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_Q);
    quickLoadAct->setStatusTip(tr("Load last scene"));
    connect(quickLoadAct, SIGNAL(triggered()), this, SLOT(quickLoad()));

    //saveSceneAct
    saveSceneAct = new QAction(tr("&Save Scene"), this);
    saveSceneAct->setShortcut(Qt::CTRL + Qt::Key_S);
    saveSceneAct->setStatusTip(tr("Save Scene"));
    connect(saveSceneAct, SIGNAL(triggered()), this, SLOT(saveScene()));

    //closeAct
    closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(Qt::CTRL + Qt::Key_C);
    closeAct->setStatusTip(tr("Close Scene"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    //exitAct
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));

    //frontAct
    frontAct = new QAction(tr("Front"), this);
    frontAct->setStatusTip(tr("Camera Front View"));
    connect(frontAct, SIGNAL(triggered()), this, SLOT(front()));

    //topAct
    topAct = new QAction(tr("Top"), this);
    topAct->setStatusTip(tr("Camera Top View"));
    connect(topAct, SIGNAL(triggered()), this, SLOT(top()));

    //sideAct
    sideAct = new QAction(tr("Side"), this);
    sideAct->setStatusTip(tr("Camera Side View"));
    connect(sideAct, SIGNAL(triggered()), this, SLOT(side()));

    //startSimulationAct
    startSimulationAct = new QAction(tr("Start Simulation"), this);
    connect(startSimulationAct, SIGNAL(triggered()), this, SLOT(startSimulation()));

    //stopSimulationAct
    stopSimulationAct = new QAction(tr("Stop Simulation"), this);
    connect(stopSimulationAct, SIGNAL(triggered()), this, SLOT(stopSimulation()));

    //simulateSingleStepAct
    simulateSingleStepAct = new QAction(tr("Simulate Single Step"), this);
    simulateSingleStepAct->setStatusTip(tr("Simulates one step at a time"));
    connect(simulateSingleStepAct, SIGNAL(triggered()), this, SLOT(simulateSingleStep()));

    //renderDuringSimulationAct
    renderDuringSimulationAct = new QAction(tr("Render during Simulation"), this);
    connect(renderDuringSimulationAct, SIGNAL(triggered()), this, SLOT(renderDuringSimulation()));

    //whiteAct
    whiteAct = new QAction(tr("White"), this);
    connect(whiteAct, SIGNAL(triggered()), this, SLOT(white()));

    //BlackAct
    blackAct = new QAction(tr("Black"), this);
    connect(blackAct, SIGNAL(triggered()), this, SLOT(black()));

    //resetCameraAct
    resetCameraAct = new QAction(tr("Reset Camera"), this);
    resetCameraAct->setStatusTip(tr("Reset Camera Angle"));
    connect(resetCameraAct, SIGNAL(triggered()), this, SLOT(resetCamera()));

    //xga1024x768Act
    xga1024x768Act = new QAction(tr("XGA 1024 x 768"), this);
    connect(xga1024x768Act, SIGNAL(triggered()), this, SLOT(xga1024x768()));

    //vga640x480Act
    vga640x480Act = new QAction(tr("VGA 640 x 768"), this);
    connect(vga640x480Act, SIGNAL(triggered()), this, SLOT(vga640x480()));

    //hd1280x720Act
    hd1280x720Act = new QAction(tr("HD 1280 x 720"), this);
    connect(hd1280x720Act, SIGNAL(triggered()), this, SLOT(hd1280x720()));

    //aboutAct
    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(Qt::Key_F1);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    //fileMenu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(quickLoadAct);
    fileMenu->addAction(saveSceneAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    //viewMenu
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(frontAct);
    viewMenu->addAction(topAct);
    viewMenu->addAction(sideAct);

    //simulationMenu
    simulationMenu = menuBar()->addMenu(tr("Sim&ulate"));
    simulationMenu->addAction(startSimulationAct);
    simulationMenu->addAction(stopSimulationAct);
    simulationMenu->addSeparator();
    simulationMenu->addAction(simulateSingleStepAct);

    //settingsMenu
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(renderDuringSimulationAct);
    //adding background menu in settings menu
    //backgroundMenu
    backgroundMenu = settingsMenu->addMenu(tr("Background"));
    backgroundMenu->addAction(whiteAct);
    backgroundMenu->addAction(blackAct);
    //settings Menu contd...
    settingsMenu->addAction(resetCameraAct);

    //renderMenu
    renderMenu = menuBar()->addMenu(tr("&Render"));
    renderMenu->addAction(xga1024x768Act);
    renderMenu->addAction(vga640x480Act);
    renderMenu->addAction(hd1280x720Act);

    //helpMenu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createOsgWindow()
{
    viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
    setCentralWidget(viewWidget);
//    osgViewer::View* cameraView = createView(1000, 150, 400, 400, osgDB::readNodeFile("../grip2/data/robot.osg"));
//    viewWidget->addView(cameraView);
}

void MainWindow::createTreeView()
{
    tree = new QDockWidget(tr("Object Viewer"), this);
    //tree->setFloating(true);
    treeviewer = new TreeView(tree);
    tree->setWidget(treeviewer->treeView);
    addDockWidget(Qt::RightDockWidgetArea, tree);
}
