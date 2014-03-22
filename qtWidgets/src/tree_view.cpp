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


#include <QtGui>
#include "tree_view.h"
#include <iostream>
#include <cstdio>

//include icons
#include "icons/fixed.xpm"
#include "icons/free.xpm"
#include "icons/object.xpm"
#include "icons/prism.xpm"
#include "icons/revol.xpm"
#include "icons/robot.xpm"

// DART includes
#include <dart/utils/urdf/DartLoader.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/FreeJoint.h>
#include <dart/dynamics/PrismaticJoint.h>
#include <dart/dynamics/RevoluteJoint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/simulation/World.h>

using namespace dart;
using namespace std;

Tree_View::Tree_View(QWidget *parent, TreeViewReturn* active_item) :QDockWidget(parent), ui(new Ui::Tree_View)
{
    activeItem = active_item;
    ui->setupUi(this);
    ui_treeWidget = ui->treeWidget;
    ui_treeWidget->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui_treeWidget->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui_treeWidget->header()->setStretchLastSection(false);
    connect(ui_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeView_itemSelected(QTreeWidgetItem*,int)));

    ui_checkBox = ui->checkBox;
    connect(ui_checkBox, SIGNAL(stateChanged(int)), this, SLOT(nameChange_BodyNode_Joint(int)));
}

Tree_View::~Tree_View()
{
    delete ui;
}

void Tree_View::treeView_itemSelected(QTreeWidgetItem * item, int column)
{
    TreeViewReturn* val = item->data(0, Qt::UserRole).value<TreeViewReturn*>();
    activeItem = val;
    emit itemSelected(activeItem);
}

TreeViewReturn* Tree_View::getActiveItem()
{
 return activeItem;
}

QTreeWidgetItem* Tree_View::addParent(dynamics::Skeleton* skel, QIcon icon)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui_treeWidget);
    itm->setText(0, QString::fromStdString(skel->getName()));
    itm->setIcon(0, icon);

    TreeViewReturn* ret = new TreeViewReturn();
    ret->object = skel;
    ret->dType = Return_Type_Robot;

    QVariant var;
    var.setValue(ret);
    itm->setData(0, Qt::UserRole, var);

    ui_treeWidget->addTopLevelItem(itm);
    return itm;
}

QTreeWidgetItem* Tree_View::addChildItem(dynamics::BodyNode* node, QTreeWidgetItem* parent, QIcon icon)
{
    if(parent != NULL)
    {
        QTreeWidgetItem *childitm = new QTreeWidgetItem();
        childitm->setText(0, QString::fromStdString(node->getName()));
        childitm->setIcon(0,icon);

        TreeViewReturn* ret = new TreeViewReturn();
        ret->object = node;
        ret->dType = Return_Type_Node;

        QVariant var;
        var.setValue(ret);
        childitm->setData(0, Qt::UserRole, var);

        parent->addChild(childitm);
        return childitm;
    }
    else
    {
        cout<<"Parent is NULL"<<endl;
    }
}

QTreeWidgetItem* Tree_View::buildTree(dynamics::BodyNode* node, QTreeWidgetItem* prev, QTreeWidgetItem* parent, bool chain)
{
    QPixmap fixedIcon((const char**) fixed_xpm);
    QPixmap freeIcon((const char**) free_xpm);
    QPixmap objectIcon((const char**) object_xpm);
    QPixmap prismIcon((const char**) prism_xpm);
    QPixmap revolIcon((const char**) revol_xpm);
    QIcon icon;

    // Prismatic Joint: 1 DOF
    if(dart::dynamics::PrismaticJoint* joint = dynamic_cast<dart::dynamics::PrismaticJoint*>(node->getParentJoint()))
        icon = QIcon(prismIcon);

    // Revolute Joint: 1 DOF
    else if(dart::dynamics::RevoluteJoint* joint = dynamic_cast<dart::dynamics::RevoluteJoint*>(node->getParentJoint()))
        icon = QIcon(revolIcon);

    // Floating Joint: 6 DOF
    else if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(node->getParentJoint()))
        icon = QIcon(freeIcon);

    //Fixed Joint: 0 DOF
    else if(dart::dynamics::WeldJoint* joint = dynamic_cast<dart::dynamics::WeldJoint*>(node->getParentJoint()))
        icon = QIcon(fixedIcon);

    else
        icon = QIcon(objectIcon);


    QTreeWidgetItem* new_parent = parent;
    if (node->getNumChildBodyNodes() == 1)
    {
        if (node->getChildBodyNode(0)->getNumChildBodyNodes() == 1 && !chain)
            prev = new_parent = addChildItem(node, parent, QIcon(icon));
        else
        {
            prev = addChildItem(node, parent, QIcon(icon));
        }
        buildTree(node->getChildBodyNode(0), prev, new_parent, true);
    }
    else
    {
        prev = new_parent = addChildItem(node, parent, QIcon(icon));
        for (int i=0; i<node->getNumChildBodyNodes(); i++)
            prev = buildTree(node->getChildBodyNode(i), prev, new_parent, false);
    }
    return prev;
}

void Tree_View::populateTreeView(simulation::World *world)
{
    QPixmap robotIcon((const char**) robot_xpm);
    for (int i = 0; i<world->getNumSkeletons(); ++i)
    {
        dynamics::Skeleton* skel = world->getSkeleton(i);
        if(skel) {
            QTreeWidgetItem* parent = addParent(skel,  QIcon(robotIcon));
            buildTree(skel->getRootBodyNode(), parent, parent, false);
        } else {
            std::cerr << "Not a valid skeleton. Not building tree view. (Line " << __LINE__ << " of " << __FILE__ << std::endl;
        }
    }
}

void Tree_View::clear()
{
    while(ui_treeWidget->topLevelItemCount()) {
        delete ui_treeWidget->takeTopLevelItem(0);
    }
}

void Tree_View::name_Joint(QTreeWidgetItem* node)
{
    TreeViewReturn* val = node->data(0, Qt::UserRole).value<TreeViewReturn*>();
    dynamics::Joint* joint = ((dynamics::BodyNode*)val->object)->getParentJoint();
    node->setText(0, QString::fromStdString(joint->getName()));

    int count = node->childCount();
    if(count == 0)
        return;
    else
        for(int i= 0; i< count; i++)
            name_Joint(node->child(i));
}

void Tree_View::name_BodyNode(QTreeWidgetItem* node)
{
    TreeViewReturn* val = node->data(0, Qt::UserRole).value<TreeViewReturn*>();
    dynamics::BodyNode* bodyNode = (dynamics::BodyNode*)val->object;
    node->setText(0, QString::fromStdString(bodyNode->getName()));

    int count = node->childCount();
    if(count == 0)
        return;
    else
        for(int i= 0; i< count; i++)
            name_BodyNode(node->child(i));
}

void Tree_View::nameChange_BodyNode_Joint(int checkBoxState)
{
    int count = ui_treeWidget->topLevelItemCount();
    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* top = ui_treeWidget->topLevelItem(i);
        for(int j = 0; j<top->childCount(); j++)
        {
            QTreeWidgetItem* child = top->child(j);
            if(checkBoxState == Qt::Unchecked)
                name_BodyNode(child);
            else
                name_Joint(child);
        }
    }
}

//void Tree_View::itemSelected(TreeViewReturn* active_item)
//{
//
//}
