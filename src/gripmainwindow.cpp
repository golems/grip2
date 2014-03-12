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

///including the files for dart and osg
#include "Grid.h"
#include "DartNode.h"
#include <osg/io_utils>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/utils/urdf/DartLoader.h>


GripMainWindow::GripMainWindow() :
    MainWindow(), world(NULL), worldNode(new osgDart::DartNode(true)), simulation(new GripSimulation(true))
{
    createRenderingWindow();
    createTreeView();
    //world = new dart::simulation::World;
    createTabs();
}

GripMainWindow::~GripMainWindow()
{
}

void GripMainWindow::doLoad(string fileName)
{

    std::cerr << "Removing worldNode children" << std::endl;
    worldNode->removeAllSkeletons();

    if(world) {
        std::cerr << "Removing world skeletons" << std::endl;
        for(int i=0; i<world->getNumSkeletons(); ++i) {
            world->removeSkeleton(world->getSkeleton(i));
        }
    } else {
        world = new dart::simulation::World;
    }

    world->addSkeleton(createGround());
    world->setTimeStep(0.001);

    worldNode->addWorld(world);
    int numRobots = worldNode->addWorld(fileName);

    viewWidget->addNodeToScene(worldNode);
    worldNode->printInfo();

    simulation->setWorld(world);

    treeviewer->populateTreeView(worldNode->getWorld(), numRobots);

    cout << "--(i) Saving " << fileName << " to .lastload file (i)--" << endl;
    saveText(fileName,".lastload");
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
        simulation->startSimulation();
        // FIXME: Maybe use qsignalmapping or std::map for this
        this->getToolBar()->actions().at(4)->setVisible(true);
        this->getToolBar()->actions().at(3)->setVisible(false);
    } else {
        std::cerr << "Not simulating because there's no world yet" << std::endl;
    }

}

void GripMainWindow::stopSimulation()
{
    simulation->stopSimulation();
    // FIXME: Maybe use qsignalmapping or std::map for this
    this->getToolBar()->actions().at(4)->setVisible(false);
    this->getToolBar()->actions().at(3)->setVisible(true);
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
    std::cerr << "Adding viewer widget" << std::endl;
    viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
    viewWidget->addGrid(20, 20, 1);
    setCentralWidget(viewWidget);
}

void GripMainWindow::createTreeView()
{
    treeviewer = new Tree_View(this, activeItem);
    this->addDockWidget(Qt::RightDockWidgetArea, treeviewer);
}

void GripMainWindow::createTabs()
{
    setDockOptions(QMainWindow::AnimatedDocks);
    setDockOptions(QMainWindow::VerticalTabs);

    inspectortab = new Inspector_Tab(this, world,treeviewer);
    visualizationtab = new Visualization_Tab(this);

    this->addDockWidget(Qt::BottomDockWidgetArea, visualizationtab);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectortab);
    /*
    inspectabwidget->setFeatures(QDockWidget::DockWidgetMovable);
    inspectabwidget->setFeatures(QDockWidget::DockWidgetFloatable);
    */

    tabifyDockWidget(inspectortab, visualizationtab);
    visualizationtab->show();
    visualizationtab->raise();

    // Qt::RightDockWidgetArea,  Qt::LeftDockWidgetArea,  Qt::TopDockWidgetArea,  Qt::BottomDockWidgetArea,  Qt::AllDockWidgetArea

    /*
    QDockWidget *viztabwidget = new QDockWidget(this);
    Ui_Visualizer::setupUi(viztabwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, viztabwidget);

    QDockWidget *inspectabwidget = new QDockWidget(this);
    Ui_Inspector::setupUi(inspectabwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectabwidget);

    tabifyDockWidget(inspectabwidget, viztabwidget);
    viztabwidget->show();
    viztabwidget->raise();
    */
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
