/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Sungmoon Joo <sungmoon.joo@gmail.com>
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
#include "VisualizationTab.h"

// C++ Standard includes
#include <iostream>

// DART includes
#include <dart/collision/CollisionDetector.h>
#include <dart/constraint/ConstraintDynamics.h>
#include <dart/constraint/Constraint.h>

VisualizationTab::VisualizationTab(osgDart::DartNode *worldNode, TreeView* treeView, MainWindow *parent)
    : QDockWidget(parent), _parent(parent)
{
    _ui = new Ui::VisualizationTab;
    _worldNode = worldNode;
    _treeView = treeView;
    _selectedTreeViewItem = 0;

    _ui->setupUi(this);

    // Signals from parent to my slots
    connect(this, SIGNAL(signalSendMessage(QString)), _parent, SLOT(slotSetStatusBarMessage(QString)));

    // Signals from my widgets to my slots
    connect(_ui->checkBoxShowJointAxes, SIGNAL(toggled(bool)), this, SLOT(slotToggleJointAxesVisibility(bool)));
    connect(_ui->checkBoxShowBodyNodeFrames, SIGNAL(toggled(bool)), this, SLOT(slotToggleBodyNodeAxesVisibility(bool)));
    connect(_ui->checkBoxShowCoM, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonCoMVisibility(bool)));
    connect(_ui->checkBoxShowProjCoM, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonProjCoMVisibility(bool)));
    connect(_ui->checkBoxRenderUsingCollsionMesh, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonCollisionMeshMode(bool)));
    connect(_ui->checkBoxRenderWireframe, SIGNAL(toggled(bool)), this, SLOT(slotToggleSkeletonWireFrameMode(bool)));
    connect(_ui->sliderTransparency, SIGNAL(valueChanged(int)), this, SLOT(slotSetTransparencyValue(int)));
    connect(_ui->checkBoxShowContactForces, SIGNAL(toggled(bool)), this, SLOT(slotToggleContactForcesVisibility(bool)));

    // Signals from TreeView to my slots
    connect(_treeView, SIGNAL(itemSelected(TreeViewReturn*)), this, SLOT(slotSetTransparencySliderFromSelectedItem()));
    connect(_treeView, SIGNAL(itemSelected(TreeViewReturn*)), this, SLOT(slotSetSelectedTreeViewItem()));
}

VisualizationTab::~VisualizationTab()
{
    delete _ui;
}

void VisualizationTab::update()
{
    slotToggleJointAxesVisibility(_ui->checkBoxShowJointAxes->checkState());
    slotToggleBodyNodeAxesVisibility(_ui->checkBoxShowBodyNodeFrames->checkState());
    slotToggleSkeletonCoMVisibility(_ui->checkBoxShowCoM->checkState());
    slotToggleSkeletonProjCoMVisibility(_ui->checkBoxShowProjCoM->checkState());
    slotToggleSkeletonCollisionMeshMode(_ui->checkBoxRenderUsingCollsionMesh->checkState());
    slotToggleSkeletonWireFrameMode(_ui->checkBoxRenderWireframe->checkState());
    slotToggleContactForcesVisibility(_ui->checkBoxShowContactForces->checkState());
}

void VisualizationTab::slotToggleJointAxesVisibility(bool checked)
{
    _worldNode->setJointAxesVisible(checked);
}

void VisualizationTab::slotToggleBodyNodeAxesVisibility(bool checked)
{
    _worldNode->setBodyNodeAxesVisible(checked);
}

void VisualizationTab::slotToggleSkeletonCoMVisibility(bool checked)
{
    _worldNode->setSkeletonCoMVisible(checked);
}

void VisualizationTab::slotToggleSkeletonProjCoMVisibility(bool checked)
{
    _worldNode->setSkeletonCoMProjectedVisible(checked);
}

void VisualizationTab::slotToggleSkeletonCollisionMeshMode(bool checked)
{
    _worldNode->setSkeletonCollisionMeshOn(checked);
}

void VisualizationTab::slotToggleSkeletonWireFrameMode(bool checked)
{
    _worldNode->setSkeletonWireFrameOn(checked);
}

void VisualizationTab::slotSetTransparencyValue(int transparencyValue)
{
    if(!_selectedTreeViewItem) {
        emit signalSendMessage("VisualizationTab] No item selected in TreeView");
        return;
    }

    if(Return_Type_Robot == _selectedTreeViewItem->dType) {
        dart::dynamics::Skeleton* skel = (dart::dynamics::Skeleton*)_selectedTreeViewItem->object;
        _worldNode->setSkeletonTransparency(*skel, (float)transparencyValue/100.0);
    } else if(Return_Type_Node == _selectedTreeViewItem->dType) {
        dart::dynamics::BodyNode* node = (dart::dynamics::BodyNode*)_selectedTreeViewItem->object;
        _worldNode->setBodyNodeTransparency(*node, (float)transparencyValue/100.0);
    } else {
        emit signalSendMessage("[VisualizationTab] Unknown type selected in TreeView");
    }
}

void VisualizationTab::slotSetTransparencySliderFromSelectedItem()
{

}

void VisualizationTab::slotToggleContactForcesVisibility(bool checked)
{
    _worldNode->setContactForcesVisible(checked);
}

void VisualizationTab::slotSetSelectedTreeViewItem()
{
    // Check if we have a world
    if(!_worldNode->getNumSkeletons()) {
        return;
    }

    // Get the selected treeview item and check if it's valid
    _selectedTreeViewItem = _treeView->getActiveItem();
    if(!_selectedTreeViewItem) {
        return;
    }

}
