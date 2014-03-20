#include "visualization_tab.h"
#include <iostream>

Visualization_Tab::Visualization_Tab(osgDart::DartNode *worldNode, QWidget *parent)
 : QDockWidget(parent), _worldNode(worldNode), visualizer_ui(new Ui::Visualization_Tab)
{
    //QDockWidget *widget = new QDockWidget(this);
    //visualizer_ui.setupUi(widget);
    //setMainWidget(widget);
    //Ui_Visualizer::setupUi(this);
    //this->addDockWidget(Qt::BottomDockWidgetArea, widget);
    //widget->show();
    visualizer_ui->setupUi(this);
    connect(visualizer_ui->checkBoxShowJointAxes, SIGNAL(toggled(bool)), this, SLOT(slotToggleJointAxesVisibility(bool)));
    connect(visualizer_ui->checkBoxShowBodyNodeFrames, SIGNAL(toggled(bool)), this, SLOT(slotToggleBodyNodeAxesVisibility(bool)));
    connect(visualizer_ui->checkBoxShowCoM, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonCoMVisibility(bool)));
    connect(visualizer_ui->checkBoxShowProjCoM, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonProjCoMVisibility(bool)));
}

Visualization_Tab::~Visualization_Tab()
{
}

void Visualization_Tab::update()
{
    slotToggleJointAxesVisibility(visualizer_ui->checkBoxShowJointAxes->checkState());
    slotToggleBodyNodeAxesVisibility(visualizer_ui->checkBoxShowBodyNodeFrames->checkState());
    slotToggleSkeletonCoMVisibility(visualizer_ui->checkBoxShowCoM->checkState());
    slotToggleSkeletonProjCoMVisibility(visualizer_ui->checkBoxShowProjCoM->checkState());
}

void Visualization_Tab::slotToggleJointAxesVisibility(bool checked)
{
    if(true) {
        std::cerr << "[Visualization_Tab] Setting Joint visibility to " << (checked == false ? "False" : "True") << std::endl;
    }
    _worldNode->setJointAxesVisible(checked);
}

void Visualization_Tab::slotToggleBodyNodeAxesVisibility(bool checked)
{
    _worldNode->setBodyNodeAxesVisible(checked);
}

void Visualization_Tab::slotToggleSkeletonCoMVisibility(bool checked)
{
    _worldNode->setSkeletonCoMVisible(checked);
}

void Visualization_Tab::slotToggleSkeletonProjCoMVisibility(bool checked)
{
    _worldNode->setSkeletonCoMProjectedVisible(checked);
}
