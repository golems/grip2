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

// Local includes
#include "GripMainWindow.h"
#include "ViewerWidget.h"
#include "VisualizationTab.h"
#include "ui_VisualizationTab.h"
#include "InspectorTab.h"
#include "ui_InspectorTab.h"
#include "TreeView.h"
#include "ui_TreeView.h"
#include "DoubleSlider.h"
#include "Grid.h"
#include "Line.h"
#include "DartNode.h"

// Qt includes
#include <QtGui>

// OpenSceneGraph includes
#include <osg/io_utils>

// DART includes
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/utils/urdf/DartLoader.h>


GripMainWindow::GripMainWindow(bool debug) :
    MainWindow(),
    _debug(debug),
    world(new dart::simulation::World()),
    worldNode(new osgDart::DartNode(debug)),
    pluginList(new QList<GripTab*>),
    pluginMenu(new QMenu),
    _simulating(false),
    _playingBack(false),
    _curPlaybackTick(0),
    _playbackSpeed(5),
    _simulationDirty(false)
{
    /// object initialization
    world->setTime(0);
    playbackWidget = new PlaybackWidget(this);
    timeline = new std::vector<GripTimeslice>(0);
    simulation = new GripSimulation(world, timeline, pluginList, this, debug);
    pluginList = new QList<GripTab*>;
    pluginPathList = new QList<QString*>;
    sceneFilePath = new QString();

    /// create objects for widget classes
    createTreeView();
    createRenderingWindow();
    createTabs();

    /// load widgets in the user interface and manages the layout
    manageLayout();
    createPluginMenu();

    /// set the status bar for the Grip Window
    this->setStatusBar(this->statusBar());

    connect(this, SIGNAL(destroyed()), simulation, SLOT(deleteLater()));
}

GripMainWindow::~GripMainWindow()
{
}

void GripMainWindow::doLoad(std::string sceneFileName)
{
    sceneFilePath->fromStdString(sceneFileName);

    if (_simulating || _playingBack) {
        if (!stopSimulationWithDialog()) {
            if (_debug) std::cerr << "Not loading a new world" << std::endl;
            return;
        }
    }

    if (world->getTime() || world->getNumSkeletons()) {
        if (_debug) std::cerr << "Deleting world" << std::endl;
        this->clear();
    }

    world->setTimeStep(0.001);

    world->addSkeleton(createGround());
    worldNode->addWorld(world);
    worldNode->addWorld(sceneFileName);

    viewWidget->addNodeToScene(worldNode);

    worldNode->printInfo();

    treeviewer->populateTreeView(world);
    visualizationTab->update();

    if (_debug) std::cout << "--(i) Saving " << sceneFileName << " to ~/.griplastload file (i)--" << std::endl;
    saveText(sceneFileName, LAST_LOAD_FILE);
    inspectorTab->initializeTab();
    this->slotSetStatusBarMessage("Successfully loaded scene " + QString::fromStdString(sceneFileName));

    // Tell all the tabs that a new scene has been loaded
    for (size_t i = 0; i < pluginList->size(); ++i) {
        pluginList->at(i)->GRIPEventSceneLoaded();
    }
}

void GripMainWindow::close()
{
    std::cerr << "Closing" << std::endl;
    clear();
}

bool GripMainWindow::stopSimulationWithDialog()
{
    // Ask user if they really want to end the simulation and load a new world
    QMessageBox msgBox;

    QString action = (_simulating ? "simulation" : "playback");
    msgBox.setWindowTitle(tr("Stop " + action + " and Open Scene?"));
    msgBox.setText(tr("Ending " + action + " and opening new scene!"));
    msgBox.setInformativeText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();

    if (QMessageBox::Cancel == resp) {
        return false;
    }

    // Stop simulation
    if (_simulating) {
        simulation->stopSimulation();
        // Wait for simulation to stop by letting the event loop process
        // events until the "simulationStopped" slot is called which set the
        // _simulating flag to false.
        while (_simulating) {
            QCoreApplication::processEvents();
        }
        // Swap the start and stop buttons
        this->swapStartStopButtons();
    } else {
        this->slotPlaybackPause();
    }

    return true;
}

void GripMainWindow::clear()
{
    if (world) {
        worldNode->clear();
        while (world->getNumSkeletons()) {
            world->removeSkeleton(world->getSkeleton(0));
        }
        world->setTime(0);
        treeviewer->clear();
        simulation->reset();
        timeline->clear();
        sceneFilePath = NULL;
    }
}

void GripMainWindow::simulationStopped()
{
    if(_debug) std::cerr << "Got simulationStopped signal" << std::endl;
    _simulating = false;
    playbackWidget->ui->sliderMain->setEnabled(true);
    playbackWidget->slotUpdateSliderMinMax(0, timeline->size() - 1);
    playbackWidget->setSliderValue(timeline->size() - 1);
}

void GripMainWindow::slotSetWorldFromPlayback(int sliderTick)
{
    if (_simulating || timeline->size() <= 0) {
        playbackWidget->setSliderValue(0);
        return;
    }

    assert(sliderTick < timeline->size() && sliderTick >= 0);

    _curPlaybackTick = sliderTick;
    world->setTime(timeline->at(_curPlaybackTick).getTime());
    this->setWorldState_Issue122(timeline->at(sliderTick).getState());
    playbackWidget->slotSetTimeDisplays(world->getTime(), 0);
    playbackWidget->slotUpdateSliderMinMax(0, timeline->size() - 1);
}

void GripMainWindow::setWorldState_Issue122(const Eigen::VectorXd &_newState)
{
    // Before setting states, make sure the FreeJoints get their mT_Joints set
    for (int i = 0; i < world->getNumSkeletons(); i++) {
        int start = 2 * world->getIndex(i);
        int size = 2 * (world->getSkeleton(i)->getNumGenCoords());
        Eigen::VectorXd q = _newState.segment(start, size / 2);
        // Set config calls updateTransform() [NO updateTransform_Issue122], which correctly initializes mT_Joint for FREE JOINT
        world->getSkeleton(i)->setConfig(q);
        // The usual line. The line above is actually repeating some processing, but there is no other way, unless you want to touch DART itself
        world->getSkeleton(i)->setState(_newState.segment(start, size));
    }
}

void GripMainWindow::slotPlaybackStart()
{
    if (timeline->size() <= 0) {
        return;
    }

    if (playbackWidget->getSliderValue() == (timeline->size() - 1)) {
        this->slotPlaybackBeginning();
    }

    this->slotSetStatusBarMessage(tr("Starting playback"));

    if (_playingBack) {
        slotPlaybackPause();
    }

    _curPlaybackTick = playbackWidget->getSliderValue();
    _simulationDirty = true;

    for (size_t i = 0; i < pluginList->size(); ++i) {
        pluginList->at(i)->GRIPEventPlaybackStart();
    }

    _playingBack = true;
    this->slotPlaybackTimeStep(true);
}

void GripMainWindow::slotPlaybackPause()
{
    if (timeline->size() <= 0 || !_playingBack) {
        return;
    }

    this->slotSetStatusBarMessage(tr("Pausing playback"));

    _playingBack = false;

    for (size_t i = 0; i < pluginList->size(); ++i) {
        pluginList->at(i)->GRIPEventPlaybackStop();
    }
}

void GripMainWindow::slotPlaybackReverse()
{
    if (timeline->size() <= 0) {
        return;
    }

    if (playbackWidget->getSliderValue() == 0) {
        _curPlaybackTick = timeline->size() - 1;
        playbackWidget->setSliderValue(_curPlaybackTick);
        world->setTime(timeline->back().getTime());
        this->setWorldState_Issue122(timeline->back().getState());
        playbackWidget->slotSetTimeDisplays(world->getTime(), 0);
    }

    this->slotSetStatusBarMessage(tr("Reversing playback"));

    if (_playingBack) {
        slotPlaybackPause();
    }

    _curPlaybackTick = playbackWidget->getSliderValue();

    for (size_t i = 0; i < pluginList->size(); ++i) {
        pluginList->at(i)->GRIPEventPlaybackStart();
    }

    _playingBack = true;
    this->slotPlaybackTimeStep(false);
}

void GripMainWindow::slotPlaybackBeginning()
{
    if (timeline->size() <= 0) {
        return;
    }

    this->slotSetStatusBarMessage(tr("Setting playback to beginning"));

    _curPlaybackTick = 0;
    playbackWidget->setSliderValue(_curPlaybackTick);
    if (timeline->size() > 0) {
        world->setTime(timeline->at(_curPlaybackTick).getTime());
        this->setWorldState_Issue122(timeline->front().getState());
    }
    playbackWidget->slotSetTimeDisplays(world->getTime(), 0);
}

void GripMainWindow::slotPlaybackTimeStep(bool playForward)
{
    if (_playingBack) {

        // Call user tab functions before time step
        for (size_t i = 0; i < pluginList->size(); ++i) {
            pluginList->at(i)->GRIPEventPlaybackBeforeFrame();
        }

        // Play time step
        world->setTime(timeline->at(_curPlaybackTick).getTime());
        this->setWorldState_Issue122(timeline->at(_curPlaybackTick).getState());
        playbackWidget->slotSetTimeDisplays(world->getTime(), 0);

        std::cerr << "cur: " << _curPlaybackTick << ", " << _curPlaybackTick + _playbackSpeed << ", " << timeline->size()-1 << std::endl;
        if ((_curPlaybackTick == 0 && !playForward)
                || (_curPlaybackTick == (timeline->size() - 1) && playForward)) {
            _playingBack = false;
            std::cerr << "Done playing back" << std::endl;
        } else if (((_curPlaybackTick - _playbackSpeed) < 0 && !playForward)
                || ((_curPlaybackTick + _playbackSpeed) > (timeline->size() - 1) && playForward)) {
            _curPlaybackTick = (playForward ? timeline->size() - 1 : 0); 
        } else {
            playForward ? _curPlaybackTick = _curPlaybackTick + _playbackSpeed
                    : _curPlaybackTick = _curPlaybackTick - _playbackSpeed;
        }

        playbackWidget->setSliderValue(_curPlaybackTick);

        // Call user tab functions after time step
        for (size_t i = 0; i < pluginList->size(); ++i) {
            pluginList->at(i)->GRIPEventPlaybackAfterFrame();
        }

    } else {
        return;
    }

    QMetaObject::invokeMethod(this, "slotPlaybackTimeStep",
                              Qt::QueuedConnection, Q_ARG(bool, playForward));
}

int GripMainWindow::saveText(std::string scenepath, const QString &filename)
{
    try {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return 0;
        QTextStream out(&file);
        out << QString::fromStdString(scenepath) << "\n";
    }

    catch (const std::exception& e) {
        std::cout <<  e.what() << std::endl;
        return 0;
    }
    return 1;
}

void GripMainWindow::front()
{
    viewWidget->setToFrontView();
}

void GripMainWindow::top()
{
    viewWidget->setToTopView();
}

void GripMainWindow::side()
{
    viewWidget->setToSideView();
}


void GripMainWindow::xga1024x768(){}
void GripMainWindow::vga640x480(){}
void GripMainWindow::hd1280x720(){}


void GripMainWindow::startSimulation()
{
    if (_playingBack) {
        slotSetStatusBarMessage(tr("Stop playback first"));
        return;
    }

    // If we have a valid world, start simulating
    if (world->getNumSkeletons()) {
        if (_simulationDirty) {
            while (timeline->size() > _curPlaybackTick + 1) {
                timeline->erase(timeline->end());
            }

            // Set world back to last simulated timestep
            if (timeline->size() > 0) {
                world->setTime(timeline->at(_curPlaybackTick).getTime());
                this->setWorldState_Issue122(timeline->at(_curPlaybackTick).getState());
            }
            _simulationDirty = false;
        }

        playbackWidget->ui->sliderMain->setDisabled(true);

        _simulating = true;
        simulation->startSimulation();
        // FIXME: Maybe use qsignalmapping or std::map for this
        swapStartStopButtons();
    } else {
        slotSetStatusBarMessage(tr("Not simulating because there's no world yet"));
    }

}

void GripMainWindow::stopSimulation()
{
    simulation->stopSimulation();
    _simulating = false;
    // FIXME: Maybe use qsignalmapping or std::map for this
    swapStartStopButtons();
}

void GripMainWindow::swapStartStopButtons()
{
    if (this->_getToolBar()->actions().at(3)->isVisible()) {
        this->_getToolBar()->actions().at(3)->setVisible(false);
        this->_getToolBar()->actions().at(4)->setVisible(true);
    } else {
        this->_getToolBar()->actions().at(3)->setVisible(true);
        this->_getToolBar()->actions().at(4)->setVisible(false);
    }
}

void GripMainWindow::simulateSingleStep()
{
    simulation->simulateSingleTimeStep();
}

void GripMainWindow::renderDuringSimulation(){}

void GripMainWindow::white()
{
    viewWidget->setBackgroundColor(osg::Vec4(1, 1, 1, 1));
}

void GripMainWindow::gray()
{
    viewWidget->setBackgroundColor(osg::Vec4(.5, .5, .5, 1));
}

void GripMainWindow::black()
{
    viewWidget->setBackgroundColor(osg::Vec4(0, 0, 0, 1));
}

void GripMainWindow::resetCamera()
{
    viewWidget->setCameraToHomePosition();
}

void GripMainWindow::createRenderingWindow()
{
    viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
    viewWidget->addGrid(20, 20, 1);
}

void GripMainWindow::createTreeView()
{
    treeviewer = new TreeView(this, pluginList);
}

void GripMainWindow::loadPluginDirectory(QDir pluginsDirName)
{
    std::cerr << "Plugin Path: " << pluginsDirName.absolutePath().toStdString() << std::endl;

    foreach (QString fileName, pluginsDirName.entryList(QDir::Files)) {
        std::cerr << "Entry: " << fileName.toStdString() << std::endl;
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix() != "so") {
            slotSetStatusBarMessage(tr("Incorrect file extension on plug: " + fileInfo.path() + " >>> " + fileInfo.suffix()));
            continue;
        }
        if (_debug) std::cerr << "Attempting to load plugin: " << fileInfo.absoluteFilePath().toStdString() << std::endl;
        this->loadPluginFile(pluginsDirName.absolutePath() + "/" + fileName);
    }
}

void GripMainWindow::loadPluginFile(QString pluginFileName)
{
    std::cerr << "File: " << pluginFileName.toStdString() << std::endl;
    QPluginLoader loader(pluginFileName);
    QObject* plugin = loader.instance();
    if (plugin) {
        GripTab* gt = qobject_cast<GripTab*>(plugin);
        if (gt) {
            gt->Load(treeviewer->getActiveItem(), viewWidget, world, timeline);

            QDockWidget* pluginWidget = qobject_cast<QDockWidget*>(plugin);
            if (pluginWidget == NULL) {
                if (_debug)
                    std::cerr << "is NULL" << std::endl;
            }
            else {
                this->addDockWidget(Qt::BottomDockWidgetArea, pluginWidget);
                this->tabifyDockWidget(visualizationTab, pluginWidget);
                pluginList->append(gt);
                pluginPathList->append(new QString(pluginFileName));
                if (_debug) std::cerr << "Plugin loaded " << (plugin->objectName()).toStdString() << std::endl;
                pluginMenu->addAction(pluginWidget->toggleViewAction());
            }
        }
    }
    else {
        slotSetStatusBarMessage(tr("Couldn't load plugin. " + loader.errorString()));
        if (_debug) {
            std::cerr << "Plugin could not be loaded" << std::endl;
            std::cerr << "Error: " << (loader.errorString()).toStdString() << std::endl;
        }
    }
}

void GripMainWindow::setSimulationRelativeTime(double time)
{
    playbackWidget->slotSetTimeDisplays(world->getTime(), time);
}

void GripMainWindow::createTabs()
{
    inspectorTab = new InspectorTab(this, world, treeviewer);
    visualizationTab = new VisualizationTab(worldNode, treeviewer, this);
}

dart::dynamics::Skeleton* GripMainWindow::createGround()
{
    // Add floor
    dart::dynamics::Skeleton* ground = new dart::dynamics::Skeleton();
    ground->setName("ground");

    dart::dynamics::BodyNode* node = new dart::dynamics::BodyNode("ground");
    node->setMass(1.0);

    dart::dynamics::Shape* shape = new dart::dynamics::BoxShape(Eigen::Vector3d(10.0, 10.0, 0.0001));
    shape->setColor(Eigen::Vector3d(0.5, 0.5, 1.0));
    node->addCollisionShape(shape);

    dart::dynamics::Joint* joint = new dart::dynamics::WeldJoint();
    joint->setName("groundJoint");
    joint->setTransformFromParentBodyNode(Eigen::Isometry3d::Identity());
    joint->setTransformFromChildBodyNode(Eigen::Isometry3d::Identity());
    Eigen::Isometry3d m = Eigen::Isometry3d::Identity();
    node->setParentJoint(joint);

    ground->addBodyNode(node);
    ground->setMobile(false);
    return ground;
}

void GripMainWindow::manageLayout()
{
    QWidget* widget = new QWidget(this);
    gridLayout = new QGridLayout;

    /// viewWidget initial size: 800 x 600
    gridLayout->addWidget(viewWidget, 0, 0, 1, 2);

    gridLayout->addWidget(playbackWidget, 1, 0);

    widget->setLayout(gridLayout);
    this->setCentralWidget(widget);

    /// adding tree view
    this->addDockWidget(Qt::RightDockWidgetArea, treeviewer);

    /// adding the inspector and visualization tabs
    this->setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
    this->addDockWidget(Qt::BottomDockWidgetArea, visualizationTab);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectorTab);
    tabifyDockWidget(inspectorTab, visualizationTab);
    visualizationTab->show();
    visualizationTab->raise();
}


void GripMainWindow::createPluginMenu()
{
    pluginMenu = menuBar()->addMenu(tr("&Plugins"));

    QList<QDockWidget *> dockwidgetList = qFindChildren<QDockWidget *>(this);
     if (dockwidgetList.size()) {
         for (int i = 0; i < dockwidgetList.size(); ++i) {
                 pluginMenu->addAction(dockwidgetList.at(i)->toggleViewAction());
         }
     }
}

QDomDocument* GripMainWindow::generateWorkspaceXML()
{
    QDomDocument* config = new QDomDocument();
    QDomProcessingInstruction xmlDeclaration = config->createProcessingInstruction("xml", "version=\"1.0\"");
    config->appendChild(xmlDeclaration);

    QDomElement root = config->createElement("configuration");

    // add plugins
    QDomElement plugins = config->createElement("plugins");
    root.appendChild(plugins);

    if (pluginPathList != NULL)
    {
        std::cerr << "adding plugins" << std::endl;
        for (int i = 0; i < pluginPathList->count(); ++i) {
            std::cerr << (*(pluginPathList->at(i))).toStdString() << std::endl;
            QDomElement plugin = config->createElement("plugin");
            plugin.setAttribute("ppath", *(pluginPathList->at(i)));
            plugins.appendChild(plugin);
        }
    }

    // add dockwidget status
    QDomElement dockWidgetStatus = config->createElement("dockWidgetStatus");
    root.appendChild(dockWidgetStatus);

    QList<QAction*> actionList;
    if (pluginMenu != NULL)
    {
        actionList = pluginMenu->actions();
        if (!actionList.isEmpty()) {
            for(int i = 0; i < actionList.count(); i++) {
                QAction* act = actionList.at(i);
                QDomElement dockWidget = config->createElement("dockWidget");
                dockWidget.setAttribute("name", act->text());
                dockWidget.setAttribute("isChecked", QString::number(act->isChecked()));
                dockWidgetStatus.appendChild(dockWidget);
            }
        }
    }

    // add scene file information
    QDomElement scene = config->createElement("scene");
    if (!sceneFilePath->isNull()) {
        scene.setAttribute("spath", *sceneFilePath);
        std::cerr << "spath: " << sceneFilePath->toStdString() << std::endl;
    } else {
        if (_debug) std::cerr << "SceneFilePath is NULL" << std::endl;
    }
    root.appendChild(scene);

    // to be added when necessary implementations are completed
    // add camera information
    // add light sources
    config->appendChild(root);
    return config;
}

void GripMainWindow::parseConfig(QDomDocument config)
{
    /// parse and load plugins
    QDomNodeList pluginList = config.elementsByTagName("plugin");
    for (int i = 0; i < pluginList.count(); i++) {
        QDomElement plugin = pluginList.at(i).toElement();
        QString pluginPath = plugin.attribute("ppath");
        loadPluginFile(pluginPath);
    }

    /// parse and set qDockWidget states
    QList<QAction*> actionList;
    actionList = pluginMenu->actions();
    QDomNodeList dockWidgetList = config.elementsByTagName("dockWidget");
    if (!actionList.isEmpty()) {
        for (int i = 0; i < dockWidgetList.count(); i++) {
            QDomElement dockWidget = dockWidgetList.at(i).toElement();
            QString dockWidgetName = dockWidget.attribute("name");
            bool isChecked = dockWidget.attribute("isChecked").toInt();

            QList<QDockWidget *> dockWidgets = qFindChildren<QDockWidget *>(this);
            if (dockWidgets.size()) {
                for (int i = 0; i < dockWidgets.size(); ++i) {
                    if ( QString(dockWidgets.at(i)->windowTitle()).compare(dockWidgetName) == 0) {
                        dockWidgets.at(i)->setHidden(!isChecked);
                    }
                }
            }

            for (int j = 0; j < actionList.count(); j++){
                if (actionList.at(i)->text().compare(dockWidgetName) == 0)
                    actionList.at(i)->setChecked(isChecked);
            }
        }
    }

    /// parse and load scene
    QDomNodeList sceneList = config.elementsByTagName("scene");
    for (int i = 0; i < sceneList.count(); i++) {
        QDomElement scene = sceneList.at(i).toElement();
        QString scenePath = scene.attribute("spath");
        doLoad(scenePath.toStdString());
    }

    /// parse and load cameras
    /// parse and load lightsources
}
