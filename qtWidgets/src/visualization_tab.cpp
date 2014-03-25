/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael X. Grey <mxgrey@gatech.edu>
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


#include "visualization_tab.h"
#include <iostream>

Visualization_Tab::Visualization_Tab(osgDart::DartNode *worldNode, TreeView* treeView, QWidget *parent)
    : QDockWidget(parent), _worldNode(worldNode), _treeView(treeView), visualizer_ui(new Ui::Visualization_Tab)
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
    connect(visualizer_ui->checkBoxRenderUsingCollsionMesh, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonCollisionMeshMode(bool)));
    connect(visualizer_ui->checkBoxRenderWireframe, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonWireFrameMode(bool)));
    connect(visualizer_ui->sliderTranparency, SIGNAL(valueChanged(int)), this, SLOT(slotSetTransparencyValue(int)));
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
    slotToggleSkeletonCollisionMeshMode(visualizer_ui->checkBoxRenderUsingCollsionMesh->checkState());
    slotToggleSkeletonWireFrameMode(visualizer_ui->checkBoxRenderWireframe->checkState());
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

void Visualization_Tab::slotToggleSkeletonCollisionMeshMode(bool checked)
{
    _worldNode->setSkeletonCollisionMeshOn(checked);
}

void Visualization_Tab::slotToggleSkeletonWireFrameMode(bool checked)
{
    _worldNode->setSkeletonWireFrameOn(checked);
}

void Visualization_Tab::slotSetTransparencyValue(int transparencyValue)
{
    TreeViewReturn* selectedItem = _treeView->getActiveItem();
    if(!selectedItem) {
        std::cerr << "[VisualizationTab] No item selected" << std::endl;
        return;
    }
    if(Return_Type_Robot == selectedItem->dType) {
        dart::dynamics::Skeleton* skel = (dart::dynamics::Skeleton*)selectedItem->object;
        _worldNode->setSkeletonTransparency(*skel, (float)transparencyValue/100.0);
    } else if(Return_Type_Node == selectedItem->dType) {
        dart::dynamics::BodyNode* node = (dart::dynamics::BodyNode*)selectedItem->object;
        _worldNode->setBodyNodeTransparency(*node, (float)transparencyValue/100.0);
    } else {
        std::cerr << "[VisualizationTab] Unknown type selected in tree view" << std::endl;
    }
}
