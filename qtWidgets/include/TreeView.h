#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QDockWidget>
#include "ui_TreeView.h"
#include "TreeViewReturn.h"
#include <dart/simulation/World.h>
#include <dart/dynamics/BodyNode.h>

using namespace dart;

namespace Ui {
class TreeView;
}


class TreeView : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit TreeView(QWidget *parent = 0, TreeViewReturn* active_item = NULL);
    void populateTreeView(simulation::World* world);
    void clear();
    ~TreeView();
    TreeViewReturn* getActiveItem();

signals:
    void itemSelected(TreeViewReturn* active_item);

private slots:
    void nameChange_BodyNode_Joint(int checkBoxState);
    void treeView_itemSelected(QTreeWidgetItem * item, int column);
    
private:
    Ui::TreeView *ui;
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
