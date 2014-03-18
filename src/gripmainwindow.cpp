#include "gripmainwindow.h"

#include <QtGui>

///including the osg gl widget
#include "ViewerWidget.h"

///including tab files
#include "visualization_tab.h"
#include "ui_visualization_tab.h"

#include "inspector_tab.h"
#include "ui_inspector_tab.h"

#include "tree_view.h"
#include "ui_tree_view.h"

#include "time_display.h"
#include "ui_time_display.h"

#include "doubleslider.h"

///including the files for dart and osg
#include "Grid.h"
#include "Line.h"
#include "DartNode.h"
#include <osg/io_utils>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/utils/urdf/DartLoader.h>


GripMainWindow::GripMainWindow() :
    MainWindow(),
    world(new dart::simulation::World()),
    worldNode(new osgDart::DartNode(true)),
    simulation(new GripSimulation(world, this, true)),
    _simulating(false)
{
    createRenderingWindow();
    createTreeView();
//    createSliders();
    createTimeDisplays();
    createTabs();
    this->setStatusBar(this->statusBar());

    connect(this, SIGNAL(destroyed()), simulation, SLOT(deleteLater()));
}

GripMainWindow::~GripMainWindow()
{
}

void GripMainWindow::doLoad(string fileName)
{
    if(_simulating) {
        if(!stopSimulationWithDialog()) {
            std::cerr << "Not loading a new world" << std::endl;
            return;
        }
    }

    if(world->getTime() || world->getNumSkeletons()) {
        std::cerr << "Deleting world" << std::endl;
        this->clear();
    }

    world->setTimeStep(0.001);

    world->addSkeleton(createGround());
    worldNode->addWorld(world);
    worldNode->addWorld(fileName);

    viewWidget->addNodeToScene(worldNode);

    worldNode->printInfo();

    treeviewer->populateTreeView(world);

    cout << "--(i) Saving " << fileName << " to .lastload file (i)--" << endl;
    saveText(fileName,".lastload");
}

bool GripMainWindow::stopSimulationWithDialog()
{
    // Ask user if they really want to end the simulation and load a new world
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Stop Simulation and Open Scene?"));
    msgBox.setText(tr("Ending simulation and opening new scene!"));
    msgBox.setInformativeText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();

    switch(resp) {
        case QMessageBox::Cancel: {
            return false;
        }
        case QMessageBox::Ok:
        default: {
            break;
        }
    }

    // Stop simulation
    simulation->stopSimulation();

    // Wait for simulation to stop by letting the event loop process
    // events until the "simulationStopped" slot is called which set the
    // _simulating flag to false.
    while(_simulating) {
        QCoreApplication::processEvents();
    }

    // Swap the start and stop buttons
    this->swapStartStopButtons();

    return true;
}

void GripMainWindow::clear()
{
    if(world) {
        worldNode->clear();
        while(world->getNumSkeletons()) {
            world->removeSkeleton(world->getSkeleton(0));
        }
        world->setTime(0);
        treeviewer->clear();
        simulation->reset();
    }
    std::cerr << "world state: \n" << world->getState().transpose() << std::endl;
}

void GripMainWindow::simulationStopped()
{
    std::cerr << "Got simulationStopped signal" << std::endl;
    _simulating = false;
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

void GripMainWindow::slotAddTimesliceToTimeline(const GripTimeslice& timeslice)
{
    std::cerr << "Timeline: " << timeline.size() << std::endl;
    timeline.push_back(timeslice);
    ostringstream msg;
    msg << "New timeslice. Size = " << timeline.size();
    this->setMessageSlot(tr(msg.str().c_str()));
}

int GripMainWindow::saveText(string scenepath, const char* llfile)
{
    try
    {
        QFile file(llfile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return 0;
        QTextStream out(&file);
        out << QString::fromStdString(scenepath) << "\n";
    }

    catch (const std::exception& e)
    {
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
    if(world) {
        _simulating = true;
        simulation->startSimulation();
        // FIXME: Maybe use qsignalmapping or std::map for this
        swapStartStopButtons();
    } else {
        std::cerr << "Not simulating because there's no world yet" << std::endl;
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
    if(this->getToolBar()->actions().at(3)->isVisible()) {
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
#include "DartVisual.h"

void GripMainWindow::createRenderingWindow()
{
    std::cerr << "Adding viewer widget" << std::endl;
    viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
    viewWidget->addGrid(20, 20, 1);
    this->setCentralWidget(viewWidget);
//    osg::MatrixTransform* tf = new osg::MatrixTransform;
//    osg::Matrix m;
////    m.makeTranslate(0, 0, .5);
//    m.setRotate(osg::Quat(-M_PI/4, osg::Vec3(0, 1, 0)));
//    std::cerr << "M \n" << m << std::endl;
//    tf->setMatrix(m);
//    osg::Geode* g = new osg::Geode;
//    osgGolems::Line* l = new osgGolems::Line(osgGolems::LINE_WITH_ARROWS);
//    l->setColor(osg::Vec4f(1, 0, 1, 1));
//    g->addDrawable(l);
//    tf->addChild(g);
//    tf->addChild(new osgDart::DartVisuals);
//    viewWidget->addNodeToScene(tf);
}

void GripMainWindow::createTreeView()
{
    treeviewer = new Tree_View(this, activeItem);
    this->addDockWidget(Qt::RightDockWidgetArea, treeviewer);
}

void GripMainWindow::createTabs()
{
    //setDockOptions(QMainWindow::AnimatedDocks);
    //setDockOptions(QMainWindow::VerticalTabs);
    //setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
    //setDockOptions(QMainWindow::AllowNestedDocks);

    inspectortab = new Inspector_Tab(this, world,treeviewer);
    visualizationtab = new Visualization_Tab(this);

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
    inspectortab->setAllowedAreas(Qt::BottomDockWidgetArea);

    //visualizationtab->setFeatures(QDockWidget::DockWidgetMovable);
    //visualizationtab->setFeatures(QDockWidget::DockWidgetFloatable);
    //visualizationtab->setFeatures(QDockWidget::DockWidgetClosable);
    visualizationtab->setAllowedAreas(Qt::BottomDockWidgetArea);//                  setFeatures(QDockWidget::DockWidgetClosable);

    this->addDockWidget(Qt::BottomDockWidgetArea, visualizationtab);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectortab);

    tabifyDockWidget(inspectortab, visualizationtab);
    visualizationtab->show();
    visualizationtab->raise();
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
    this->addDockWidget(Qt::RightDockWidgetArea, simulation_time_display);

}
