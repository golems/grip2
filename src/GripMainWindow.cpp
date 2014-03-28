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
#include "visualization_tab.h"
#include "ui_visualization_tab.h"
#include "inspector_tab.h"
#include "ui_inspector_tab.h"
#include "TreeView.h"
#include "ui_TreeView.h"
#include "time_display.h"
#include "ui_time_display.h"
#include "doubleslider.h"
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
    _simulating(false),
    _playingBack(false),
    _curPlaybackTick(0),
    _playbackSpeed(1),
    _simulationDirty(false)
{
    world->setTime(0);
    timeline = new std::vector<GripTimeslice>(0);
    simulation = new GripSimulation(world, timeline, pluginList, this, debug);
    createRenderingWindow();
    createTreeView();
    createTimeDisplays();
    createPlaybackSliders();
    createTabs();
    pluginList = new QList<GripTab*>;
    manageLayout();

    this->setStatusBar(this->statusBar());

    connect(this, SIGNAL(destroyed()), simulation, SLOT(deleteLater()));
}

GripMainWindow::~GripMainWindow()
{
}

void GripMainWindow::doLoad(string fileName)
{
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
    worldNode->addWorld(fileName);

    viewWidget->addNodeToScene(worldNode);

    worldNode->printInfo();

    treeviewer->populateTreeView(world);
    visualizationtab->update();

    if (_debug) cout << "--(i) Saving " << fileName << " to .lastload file (i)--" << endl;
    saveText(fileName,".lastload");
    inspectortab->initializeTab();
    this->slotSetStatusBarMessage("Successfully loaded scene " + QString::fromStdString(fileName));
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
    }
}

void GripMainWindow::simulationStopped()
{
    if(_debug) std::cerr << "Got simulationStopped signal" << std::endl;
    _simulating = false;
    playbackSlider->setEnabled(true);
    playbackSlider->slotUpdateSliderMinMax(timeline->size() - 1);
    playbackSlider->setSliderValue(timeline->size() - 1);
}

void GripMainWindow::slotSetWorldFromPlayback(int sliderTick)
{
    if (_simulating || timeline->size() <= 0) {
        playbackSlider->setSliderValue(0);
        return;
    }

    assert(sliderTick < timeline->size() && sliderTick >= 0);

    world->setTime(timeline->at(_curPlaybackTick).getTime());
    this->setWorldState_Issue122(timeline->at(sliderTick).getState());
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

    if (playbackSlider->getSliderValue() == (timeline->size() - 1)) {
        this->slotPlaybackBeginning();
    }

    this->slotSetStatusBarMessage(tr("Starting playback"));

    if (_playingBack) {
        slotPlaybackPause();
    }

    _curPlaybackTick = playbackSlider->getSliderValue();
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

    if (playbackSlider->getSliderValue() == 0) {
        _curPlaybackTick = timeline->size() - 1;
        playbackSlider->setSliderValue(_curPlaybackTick);
        world->setTime(timeline->back().getTime());
        this->setWorldState_Issue122(timeline->back().getState());
        simulation_time_display->Update_Time(world->getTime(), 0);
    }

    this->slotSetStatusBarMessage(tr("Reversing playback"));

    if (_playingBack) {
        slotPlaybackPause();
    }

    _curPlaybackTick = playbackSlider->getSliderValue();

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
    playbackSlider->setSliderValue(_curPlaybackTick);
    if (timeline->size() > 0) {
        world->setTime(timeline->at(_curPlaybackTick).getTime());
        this->setWorldState_Issue122(timeline->front().getState());
    }
    simulation_time_display->Update_Time(world->getTime(), 0);
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
        playbackSlider->setSliderValue(_curPlaybackTick);
        this->setSimulationRelativeTime(0);

        if (((_curPlaybackTick - _playbackSpeed) < 0 && !playForward)
                || ((_curPlaybackTick + _playbackSpeed) >= timeline->size() && playForward)) {
            _curPlaybackTick = (playForward ? 0 : timeline->size() - 1);
            _playingBack = false;
        } else {
            playForward ? _curPlaybackTick = _curPlaybackTick + _playbackSpeed
                    : _curPlaybackTick = _curPlaybackTick - _playbackSpeed;
        }

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

void GripMainWindow::setSimulationRelativeTime(double time)
{
    //FIXME Attach to the time info widgets
    // use input parameter time for the relative time box
    // use world->getTime() for the simulation time box
    // emit GripMainWindow::testemit(time);
    //qDebug() << time;
    //emit sim_time_changed(world->getTime());
    simulation_time_display->Update_Time(world->getTime(),time);
}

int GripMainWindow::saveText(string scenepath, const char* llfile)
{
    try {
        QFile file(llfile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return 0;
        QTextStream out(&file);
        out << QString::fromStdString(scenepath) << "\n";
    }

    catch (const std::exception& e) {
        cout <<  e.what() << endl;
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

        playbackSlider->setDisabled(true);

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
    if (this->getToolBar()->actions().at(3)->isVisible()) {
        this->getToolBar()->actions().at(3)->setVisible(false);
        this->getToolBar()->actions().at(4)->setVisible(true);
    } else {
        this->getToolBar()->actions().at(3)->setVisible(true);
        this->getToolBar()->actions().at(4)->setVisible(false);
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
//    this->setCentralWidget(viewWidget);

}

void GripMainWindow::createTreeView()
{
    treeviewer = new TreeView(this, activeItem);
//    this->addDockWidget(Qt::RightDockWidgetArea, treeviewer);
}

void GripMainWindow::loadPluginDirectory(QDir pluginsDirName)
{
    std::cerr << "PPath: " << pluginsDirName.absolutePath().toStdString() << std::endl;

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
    std::cerr << "FileFile: " << pluginFileName.toStdString() << std::endl;
    QPluginLoader loader(pluginFileName);
    QObject* plugin = loader.instance();
    if (plugin) {
        if (_debug) std::cerr << "Plugin loaded " << (plugin->objectName()).toStdString() << std::endl;
        GripTab* gt = qobject_cast<GripTab*>(plugin);
        pluginList->append(gt);
        if (gt)
        {
            gt->Load(activeItem, viewWidget);

            QDockWidget* pluginWidget = qobject_cast<QDockWidget*>(plugin);
            if (pluginWidget == NULL)
                if (_debug) std::cerr << "is NULL" << std::endl;
            else
                this->addDockWidget(Qt::BottomDockWidgetArea, pluginWidget);

            this->tabifyDockWidget(visualizationtab, pluginWidget);
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

void GripMainWindow::createTabs()
{
    //setDockOptions(QMainWindow::AnimatedDocks);
    //setDockOptions(QMainWindow::VerticalTabs);
    //setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

//    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
    //setDockOptions(QMainWindow::AllowNestedDocks);

    inspectortab = new Inspector_Tab(this, world,treeviewer);
    visualizationtab = new Visualization_Tab(worldNode, treeviewer, this);

//    QDockWidget* emptyTitle1 = new QDockWidget();
//    QDockWidget* emptyTitle2 = new QDockWidget();
//    visualizationtab->setTitleBarWidget(emptyTitle1);
//    inspectortab->setTitleBarWidget(emptyTitle2);
//    QDockWidget* emptyTitle1 = new QDockWidget();
//    QDockWidget* emptyTitle2 = new QDockWidget();
//    delete emptyTitle1;
//    delete emptyTitle2;

    //inspectortab->setFeatures(QDockWidget::DockWidgetMovable);
    //inspectortab->setFeatures(QDockWidget::DockWidgetFloatable);
    //inspectortab->setFeatures(QDockWidget::DockWidgetClosable);

//    inspectortab->setAllowedAreas(Qt::BottomDockWidgetArea);

    //visualizationtab->setFeatures(QDockWidget::DockWidgetMovable);
    //visualizationtab->setFeatures(QDockWidget::DockWidgetFloatable);
    //visualizationtab->setFeatures(QDockWidget::DockWidgetClosable);

//    visualizationtab->setAllowedAreas(Qt::BottomDockWidgetArea); //setFeatures(QDockWidget::DockWidgetClosable);

//    this->addDockWidget(Qt::BottomDockWidgetArea, visualizationtab);
//    this->addDockWidget(Qt::BottomDockWidgetArea, inspectortab);

//     tabifyDockWidget(inspectortab, visualizationtab);

//    visualizationtab->show();
//    visualizationtab->raise();

    //visualizationtab->setFloating(false);
    //inspectortab->setFloating(false);

    //QDockWidget *viztabwidget = new QDockWidget(this);
    //Ui_Visualizer::setupUi(viztabwidget);
    //this->addDockWidget(Qt::BottomDockWidgetArea, viztabwidget);

    //QDockWidget *inspectabwidget = new QDockWidget(this);
    //Ui_Inspector::setupUi(inspectabwidget);
    //this->addDockWidget(Qt::BottomDockWidgetArea, inspectabwidget);

    //tabifyDockWidget(inspectabwidget, viztabwidget);
    //viztabwidget->show();
    //viztabwidget->raise();
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


void GripMainWindow::createTimeDisplays()
{
    simulation_time_display = new Time_Display(this);
//    this->addDockWidget(Qt::RightDockWidgetArea, simulation_time_display);

}

void GripMainWindow::createPlaybackSliders()
{
    playbackSlider = new PlaybackSlider(this);
//    playbackSlider->setTitleBarWidget(new QWidget());

}


void GripMainWindow::manageLayout()
{
//    this->setDockOptions(QMainWindow::AllowTabbedDocks);
//    this->setDockOptions(QMainWindow::AllowNestedDocks);

//    QSizePolicy policy;
//    policy = viewWidget->sizePolicy();
//    policy.setHorizontalPolicy(QSizePolicy::Expanding);
//    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
//    viewWidget->setSizePolicy(policy);
    viewWidget->setMinimumWidth(500);
    viewWidget->setMinimumHeight(250);
    viewWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    treeviewer->setMinimumWidth(200);
    treeviewer->setMaximumWidth(210);
    treeviewer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QMainWindow *dummyTreeViewer = new QMainWindow;
    dummyTreeViewer->setCentralWidget(new QWidget());
    dummyTreeViewer->setMaximumWidth(210);
    //dummyTreeViewer->setMinimumWidth(200);
    dummyTreeViewer->addDockWidget(Qt::LeftDockWidgetArea, treeviewer);
    dummyTreeViewer->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(viewWidget);
    topLayout->addWidget(dummyTreeViewer);
    topLayout->setSpacing(0);
    topLayout->setMargin(0);
    topLayout->setContentsMargins(0,0,0,0);


    playbackSlider->setTitleBarWidget(new QWidget());
    playbackSlider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    simulation_time_display->setTitleBarWidget(new QWidget());
    simulation_time_display->setStyleSheet("font: 11pt \"Ubuntu\";color:rgb(0,0,0);");
    simulation_time_display->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addWidget(playbackSlider);
    midLayout->addWidget(simulation_time_display);
    midLayout->addWidget(viewWidget);
    midLayout->setSpacing(0);
    midLayout->setMargin(0);
    midLayout->setContentsMargins(0,0,0,0);

    QDockWidget *Combo = new QDockWidget;
    QWidget *dummyWidgetForCombo = new QWidget;
    dummyWidgetForCombo->setLayout(midLayout);

    Combo->setWidget(dummyWidgetForCombo);
    Combo->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    Combo->setWindowTitle(QString("Slider Timer Combo"));
    Combo->setStyleSheet("font: 0.5pt \"Ubuntu\";color:rgb(255, 255, 255);");

    QMainWindow *dummySliderTimerCombo = new QMainWindow;
    dummySliderTimerCombo->setCentralWidget(new QWidget());
    dummySliderTimerCombo->addDockWidget(Qt::BottomDockWidgetArea, Combo);
    dummySliderTimerCombo->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

//    QMainWindow *tabs = new QMainWindow;
//    tabs->setCentralWidget(new QWidget());
//    tabs->setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
//    tabs->addDockWidget(Qt::BottomDockWidgetArea, visualizationtab);
//    tabs->addDockWidget(Qt::BottomDockWidgetArea, inspectortab);
//    tabs->tabifyDockWidget(inspectortab, visualizationtab);
//    tabs->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

//    visualizationtab->show();
//    visualizationtab->raise();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    //mainLayout->addWidget(slider_timerCombo);
    mainLayout->addWidget(dummySliderTimerCombo);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0,0,0,0);
    //mainLayout->addWidget(tabs);

    QWidget *layoutManager = new QWidget;
    layoutManager->setLayout(mainLayout);

    this->setCentralWidget(layoutManager);
    this->setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
    this->addDockWidget(Qt::BottomDockWidgetArea, visualizationtab);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectortab);
    tabifyDockWidget(inspectortab, visualizationtab);
    visualizationtab->show();
    visualizationtab->raise();


    QMenu *dockwidgetMenu = menuBar()->addMenu(tr("&Dockwidgets"));

    QList<QDockWidget *> dockwidgets = qFindChildren<QDockWidget *>(this);
     if (dockwidgets.size()) {
         //menu1 = new QMenu(this);
         for (int i = 0; i < dockwidgets.size(); ++i) {
             if ( QString(dockwidgets.at(i)->name()) == QString("PlaybackSlider")
                 || QString(dockwidgets.at(i)->name()) == QString("Time_Display")) {

                 // skip //
             }
             else {
                 dockwidgetMenu->addAction(dockwidgets.at(i)->toggleViewAction());
             }

         }
     }


}
