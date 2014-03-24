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


#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QDockWidget>
#include "ui_tree_view.h"
#include <dart/simulation/World.h>
#include <dart/dynamics/BodyNode.h>

using namespace dart;

namespace Ui {
class Tree_View;
}

enum DataType
{
    Return_Type_Robot,
    Return_Type_Node
};

/**
 * \class TreeViewReturn tree_view.h
 * \brief This class is used to return the object that is selected in the
 * Object Explorer or Tree View.
 */
class TreeViewReturn
{
public:
    /**
     * \brief Constructor for TreeViewReturn
     */
    TreeViewReturn(){}

    ///used to store the Skeleton object or the BodyNode object
    void* object;

    ///used to differentiate between the skeleton object and BodyNode object
    DataType dType;

    ///used to identify skeleton id number
    int skeletonID;
};
Q_DECLARE_METATYPE(TreeViewReturn*);

class Tree_View : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit Tree_View(QWidget *parent = 0, TreeViewReturn* active_item = NULL);
    void populateTreeView(simulation::World* world);
    void clear();
    ~Tree_View();
    TreeViewReturn* getActiveItem();

signals:
    void itemSelected(TreeViewReturn* active_item);

private slots:
    void nameChange_BodyNode_Joint(int checkBoxState);
    void treeView_itemSelected(QTreeWidgetItem * item, int column);
    
private:
    Ui::Tree_View *ui;
    QTreeWidget *ui_treeWidget;
    QCheckBox *ui_checkBox;
    TreeViewReturn* activeItem;

    QTreeWidgetItem* addParent(dynamics::Skeleton *skel, QIcon icon, int skeleton_id);
    QTreeWidgetItem* addChildItem(dynamics::BodyNode* node, QTreeWidgetItem* parent, QIcon icon, int skeleton_id);
    QTreeWidgetItem* buildTree(dynamics::BodyNode* node, QTreeWidgetItem *prev, QTreeWidgetItem *parent, bool chain, int skeleton_id);
    void name_Joint(QTreeWidgetItem* node);
    void name_BodyNode(QTreeWidgetItem* node);
};

#endif // TREE_VIEW_H
