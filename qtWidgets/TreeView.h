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

/**
 * \file TreeView.h
 * \brief Class for managing and displaying a tree view of the
 * objects in the world
 */

#ifndef TREE_VIEW_H
#define TREE_VIEW_H

// Qt includes
#include <QDockWidget>

// Local includes
#include "ui_TreeView.h"
#include "TreeViewReturn.h"
#include "GripTab.h"

// DART includes
#include <dart/simulation/World.h>
#include <dart/dynamics/BodyNode.h>

/**
 * \namespace User Interface namespace for all the widgets
 */
namespace Ui {
    /**
     * \class TreeView TreeView.h
     * \brief Class containing the widgets
     */
    class TreeView;
}

/**
 * \class TreeView TreeView.h
 * \brief Class for the tree view which displays and manages the object in the
 * simulation world
 */
class TreeView : public QDockWidget
{
    /// MetaObject macro for using signals and slots
    Q_OBJECT
    
public:
    explicit TreeView(QWidget *parent = 0, QList<GripTab*>* tabs = NULL);
    void populateTreeView(dart::simulation::World *world = NULL);
    void reset();
    virtual ~TreeView();
    TreeViewReturn* getActiveItem();

signals:
    void itemSelected(TreeViewReturn *active_item);

private slots:
    void nameChangeBodyNodeJoint(int checkBoxState);
    void treeViewItemSelected(QTreeWidgetItem *item, int column);
    
private:
    Ui::TreeView *_ui;
    QTreeWidget *_ui_treeWidget;
    QCheckBox *_ui_checkBox;
    TreeViewReturn *_activeItem;
    QList<GripTab*> *_tabs;

    QTreeWidgetItem* _addParent(dart::dynamics::Skeleton *skel, QIcon icon, int skeletonId);
    QTreeWidgetItem* _addChildItem(dart::dynamics::BodyNode* node, QTreeWidgetItem *parent, QIcon icon, int skeletonId);
    QTreeWidgetItem* _buildTree(dart::dynamics::BodyNode* node, QTreeWidgetItem *prev, QTreeWidgetItem *parent, bool chain, int skeletonId);
    void _nameJoint(QTreeWidgetItem* node);
    void _nameBodyNode(QTreeWidgetItem* node);
    void _removeChildren(QTreeWidgetItem *item);

}; // end class TreeView

#endif // TREE_VIEW_H
