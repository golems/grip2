#include "gripmainwindow.h"

#include <QtGui>

///including the osg gl widget
#include "ViewerWidget.h"

///including tab files
#include "visualizer.h"
#include "ui_visualizer.h"
#include "inspector.h"
#include "ui_inspector.h"
#include "tree_view.h"
#include "ui_tree_view.h"

///including the files for dart and osg
#include "Grid.h"
#include "DartNode.h"
#include <osg/io_utils>
#include <dart/utils/urdf/DartLoader.h>


GripMainWindow::GripMainWindow() : MainWindow()
{
    createRenderingWindow();
    createTreeView();
    createTabs();
}

GripMainWindow::~GripMainWindow()
{
}

void GripMainWindow::doLoad(string fileName)
{
    worldNode = new osgDart::DartNode();
    int numRobots = worldNode->addWorld(fileName);
    viewWidget->addNodeToScene(worldNode);
    worldNode->printInfo();

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
}

void GripMainWindow::stopSimulation()
{
}

void GripMainWindow::simulateSingleStep(){}

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

    QDockWidget *viztabwidget = new QDockWidget(this);
    Ui_Visualizer::setupUi(viztabwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, viztabwidget);

    QDockWidget *inspectabwidget = new QDockWidget(this);
    Ui_Inspector::setupUi(inspectabwidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, inspectabwidget);

    tabifyDockWidget(inspectabwidget, viztabwidget);
    viztabwidget->show();
    viztabwidget->raise();
}
