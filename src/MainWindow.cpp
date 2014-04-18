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
#include <iostream>
#include <cstdio>
#include <fstream>
#include <QPluginLoader>

#include "MainWindow.h"

///including the icons for the toolbar
#include "icons/open.xpm"
#include "icons/redo.xpm"
#include "icons/simulate.xpm"
#include "icons/stop.xpm"
#include "icons/camera.xpm"
#include "icons/film.xpm"
#include "icons/frontView.xpm"
#include "icons/topView.xpm"
#include "icons/rightSideView.xpm"

MainWindow::MainWindow() : LAST_LOAD_FILE(QDir::homePath() + "/.griplastload")
{
    configFilePath = new QString(QDir::homePath() + QString("/default.gripconfig"));
    createActions();
    createMenus();
    setWindowTitle(tr("Grip"));
    resize(860, 700);
}

MainWindow::~MainWindow()
{
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

    toolbar = addToolBar("main toolbar");
    QAction *open = toolbar->addAction(QIcon(openIcon), "Open Scene (Ctrl + O)");
    QAction *redo = toolbar->addAction(QIcon(redoIcon), "Load last viewed scene (Ctrl + Shift + Q)");
    toolbar->addSeparator();
    QAction *simulate = toolbar->addAction(QIcon(simulateIcon), "Start Simulation (Ctrl + R");
    QAction *stop = toolbar->addAction(QIcon(stopIcon), "Stop Simulation (Ctrl + C");
    stop->setVisible(false);
    toolbar->addSeparator();
    QAction *camera = toolbar->addAction(QIcon(cameraIcon), "Export screenshot");
    QAction *film = toolbar->addAction(QIcon(filmIcon), "Export film sequence");
    toolbar->addSeparator();
    QAction *front = toolbar->addAction(QIcon(frontViewIcon), "View scene from front");
    QAction *top = toolbar->addAction(QIcon(topViewIcon), "View scene from top");
    QAction *rightSide = toolbar->addAction(QIcon(rightSideViewIcon), "View scene from right");

    connect(open, SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(redo, SIGNAL(triggered()), this, SLOT(quickLoad()));
    connect(simulate, SIGNAL(triggered()), this, SLOT(startSimulation()));
    connect(stop, SIGNAL(triggered()), this, SLOT(stopSimulation()));
    connect(camera, SIGNAL(triggered()), this, SLOT(camera()));
    connect(film, SIGNAL(triggered()), this, SLOT(film()));
    connect(front, SIGNAL(triggered()), this, SLOT(front()));
    connect(top, SIGNAL(triggered()), this, SLOT(top()));
    connect(rightSide, SIGNAL(triggered()), this, SLOT(side()));
}

QToolBar* MainWindow::_getToolBar()
{
    return toolbar;
}

void MainWindow::slotSetStatusBarMessage(QString msg)
{
    this->statusBar()->showMessage(msg);
}

void MainWindow::loadPluginFileWithDialog()
{
    if (true) {
        // Set file extension filters
        QStringList filters;
        filters << "Shared library (*.so)";

        QFileDialog dialog(this);
        dialog.setNameFilters(filters);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec()) {
            QString pluginPath = dialog.selectedFiles().at(0);
            std::cerr << "pluginPath: " << pluginPath.toStdString() << std::endl;
            slotSetStatusBarMessage(tr("plugin file: " + pluginPath));
            loadPluginFile(pluginPath);
        } else {
            slotSetStatusBarMessage(tr("Didn't find plugin directory"));
        }
    }
}

void MainWindow::loadPluginDirWithDialog()
{
    if (true) {
        // Set file extension filters
        QStringList filters;
        filters << "Shared library (*.so)";

        QFileDialog dialog(this);
        dialog.setNameFilters(filters);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::Directory);
        if (dialog.exec()) {
            QDir pluginsDir = QDir(dialog.selectedFiles().at(0));
            std::cerr << "pluginDir: " << pluginsDir.path().toStdString() << std::endl;
            slotSetStatusBarMessage(tr("plugin Dir: " + pluginsDir.path()));
            loadPluginDirectory(pluginsDir);
        } else {
            slotSetStatusBarMessage(tr("Didn't find plugin directory"));
        }
    }
}

void MainWindow::loadScene()
{
    QStringList fileNames; //stores the entire path of the file that it attempts to open

    QStringList filters; //setting file filters
    filters << "Scene files (*.urdf *.sdf *.world)"
            << "Any files (*)";

    //initializing the File dialog box
    //the static QFileDialog does not seem to be working correctly in Ubuntu 12.04 with unity.
    //as per the documentation it may work correctly with gnome
    //the method used below should work correctly on all desktops and is supposedly more powerful
    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if (!fileNames.isEmpty()) {
        std::cerr << "Attempting to open the following world file: " << fileNames.front().toStdString() << std::endl;
        doLoad(fileNames.front().toStdString());
    }
}

void MainWindow::quickLoad()
{
    QFile file(LAST_LOAD_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        slotSetStatusBarMessage("No saved load file");
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    doLoad(line.toStdString());
}

void MainWindow::about(){}

void MainWindow::createActions()
{
    //loadAct
    loadSceneAct = new QAction(tr("&Load Scene"), this);
    loadSceneAct->setShortcut(Qt::CTRL + Qt::Key_O);
    loadSceneAct->setStatusTip(tr("Load scene"));
    connect(loadSceneAct, SIGNAL(triggered()), this, SLOT(loadScene()));


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

    loadPluginFileAct = new QAction(tr("&Load Plugin"), this);
    loadPluginFileAct->setShortcut(Qt::CTRL + Qt::Key_L);
    loadPluginFileAct->setStatusTip(tr("Load plugin file"));
    connect(loadPluginFileAct, SIGNAL(triggered()), this, SLOT(loadPluginFileWithDialog()));

    loadPluginDirAct = new QAction(tr("&Load Plugin Directory"), this);
    loadPluginDirAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_L);
    loadPluginDirAct->setStatusTip(tr("Load all plugins in directory"));
    connect(loadPluginDirAct, SIGNAL(triggered()), this, SLOT(loadPluginDirWithDialog()));

    /// save workspace configuration action
    saveWorkspaceConfigurationAct = new QAction(tr("Save Workspace"), this);
    saveWorkspaceConfigurationAct->setStatusTip(tr("Save the configuration of the workspace"));
    connect(saveWorkspaceConfigurationAct, SIGNAL(triggered()), this, SLOT(saveWorkspace()));

    /// save new workspace configuration action
    saveNewWorkspaceConfigurationAct = new QAction(tr("Save Workspace As..."), this);
    saveNewWorkspaceConfigurationAct->setStatusTip(tr("Save a new workspace configuration"));
    connect(saveNewWorkspaceConfigurationAct, SIGNAL(triggered()), this, SLOT(saveNewWorkspace()));

    /// load workspace configuration action
    loadWorkspaceConfigurationAct = new QAction(tr("Load Workspace"), this);
    loadWorkspaceConfigurationAct->setStatusTip(tr("Load a workspace configuration"));
    connect(loadWorkspaceConfigurationAct, SIGNAL(triggered()), this, SLOT(loadWorkspace()));

    //closeAct
    closeSceneAct = new QAction(tr("&Close"), this);
    closeSceneAct->setShortcut(Qt::CTRL + Qt::Key_W);
    closeSceneAct->setStatusTip(tr("Close Scene"));
    connect(closeSceneAct, SIGNAL(triggered()), this, SLOT(close()));

    //exitAct
    exitAct = new QAction(tr("&Exit"), this);
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
    startSimulationAct->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(startSimulationAct, SIGNAL(triggered()), this, SLOT(startSimulation()));

    //stopSimulationAct
    stopSimulationAct = new QAction(tr("Stop Simulation"), this);
    stopSimulationAct->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(stopSimulationAct, SIGNAL(triggered()), this, SLOT(stopSimulation()));

    //simulateSingleStepAct
    simulateSingleStepAct = new QAction(tr("Simulate Single Step"), this);
    simulateSingleStepAct->setStatusTip(tr("Simulates one step at a time"));
    simulateSingleStepAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    connect(simulateSingleStepAct, SIGNAL(triggered()), this, SLOT(simulateSingleStep()));

    //renderDuringSimulationAct
    renderDuringSimulationAct = new QAction(tr("Render during Simulation"), this);
    connect(renderDuringSimulationAct, SIGNAL(triggered()), this, SLOT(renderDuringSimulation()));

    //whiteAct
    whiteAct = new QAction(tr("White"), this);
    connect(whiteAct, SIGNAL(triggered()), this, SLOT(white()));

    //grayAct
    grayAct = new QAction(tr("Gray"), this);
    connect(grayAct, SIGNAL(triggered()), this, SLOT(gray()));

    //BlackAct
    blackAct = new QAction(tr("Black"), this);
    connect(blackAct, SIGNAL(triggered()), this, SLOT(black()));

    //resetCameraAct
    resetCameraAct = new QAction(tr("Reset Camera"), this);
    resetCameraAct->setStatusTip(tr("Reset Camera Angle"));
    connect(resetCameraAct, SIGNAL(triggered()), this, SLOT(resetCamera()));

    //xga1024x768Act
    xga1024x768Act = new QAction(tr("XGA 1024 x 768"), this);
    xga1024x768Act->setCheckable(true);
    xga1024x768Act->setChecked(true);
    connect(xga1024x768Act, SIGNAL(triggered()), this, SLOT(xga1024x768()));

    //vga640x480Act
    vga640x480Act = new QAction(tr("VGA 640 x 768"), this);
    vga640x480Act->setCheckable(true);
    connect(vga640x480Act, SIGNAL(triggered()), this, SLOT(vga640x480()));

    //hd1280x720Act
    hd1280x720Act = new QAction(tr("HD 1280 x 720"), this);
    hd1280x720Act->setCheckable(true);
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
    fileMenu->addAction(loadSceneAct);
    fileMenu->addAction(quickLoadAct);
    fileMenu->addAction(saveSceneAct);
    fileMenu->addAction(closeSceneAct);
    fileMenu->addSeparator();
    fileMenu->addAction(loadPluginFileAct);
    fileMenu->addAction(loadPluginDirAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveWorkspaceConfigurationAct);
    fileMenu->addAction(saveNewWorkspaceConfigurationAct);
    fileMenu->addAction(loadWorkspaceConfigurationAct);
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
    backgroundMenu->addAction(grayAct);
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

void MainWindow::saveScene(){}
void MainWindow::close(){}
void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::saveWorkspace()
{
    if (!configFilePath->isNull()) {
        QDomDocument* config = generateWorkspaceXML();
        saveConfigFile(config, configFilePath);
    } else {
        saveNewWorkspace();
    }
}

void MainWindow::saveConfigFile(QDomDocument *config, QString *filename)
{
    try {
        QFile file(*filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            slotSetStatusBarMessage(QString("Unable to open file \"") + QString(QFileInfo(file).fileName()));
            std::cerr << "Unable to open file." << std::endl;
            return;
        }

        QTextStream out(&file);
        out << config->toString() << "\n";
        file.close();
        std::cerr << "Success!" << std::endl;
        *configFilePath = *filename;
    }

    catch (const std::exception& e) {
        std::cerr <<  e.what() << std::endl;
    }
}

void MainWindow::saveNewWorkspace()
{
    QDomDocument* config = generateWorkspaceXML();
    QStringList fileNames; //stores the entire path of the file that it attempts to open

    QStringList filters; //setting file filters
    filters << "Grip configuration files (*.gripconfig)"
            << "Any files (*)";

    //initializing the File dialog box
    //the static QFileDialog does not seem to be working correctly in Ubuntu 12.04 with unity.
    //as per the documentation it may work correctly with gnome
    //the method used below should work correctly on all desktops and is supposedly more powerful
    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if (!fileNames.isEmpty())
        saveConfigFile(config, &fileNames.front());
    else
        std::cerr << "No file was selected" << std::endl;
}

void MainWindow::loadWorkspace(std::string workspaceFile)
{
    QStringList fileNames; //stores the entire path of the file that it attempts to open

    if (workspaceFile.empty()) {
        QStringList filters; //setting file filters
        filters << "Grip configuration files (*.gripconfig)"
                << "Any files (*)";

        //initializing the File dialog box
        //the static QFileDialog does not seem to be working correctly in Ubuntu 12.04 with unity.
        //as per the documentation it may work correctly with gnome
        //the method used below should work correctly on all desktops and is supposedly more powerful
        QFileDialog dialog(this);
        dialog.setNameFilters(filters);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec())
            fileNames = dialog.selectedFiles();
    } else {
        fileNames.append(QString::fromStdString(workspaceFile));
    }

    if (!fileNames.isEmpty()){
        std::cerr << "Attempting to load the following configuration file: "<< fileNames.front().toStdString() << std::endl;
        try {
            QFile file(fileNames.front());
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                slotSetStatusBarMessage(QString("Unable to open file \"") + QString(QFileInfo(file).fileName()));
                std::cerr << "Unable to open file." << std::endl;
                return;
            }

            QDomDocument config;
            QString errorMsg;
            int errorLine, errorColumn;
            if(config.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
                parseConfig(config);
                *configFilePath = fileNames.front();
            }
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
    }
}
