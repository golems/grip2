#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QDockWidget>
#include "ui_tree_view.h"
#include "TreeViewReturn.h"
#include <dart/simulation/World.h>
#include <dart/dynamics/BodyNode.h>

using namespace dart;

namespace Ui {
class Tree_View;
}

class Tree_View : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit Tree_View(QWidget *parent = 0, TreeViewReturn* active_item = NULL);
    void populateTreeView(simulation::World* world, int numRobots);
    ~Tree_View();

private slots:
    void nameChange_BodyNode_Joint(int checkBoxState);
    void treeView_itemSelected(QTreeWidgetItem * item, int column);
    
private:
    Ui::Tree_View *ui;
    QTreeWidget *ui_treeWidget;
    QCheckBox *ui_checkBox;
    TreeViewReturn* activeItem;

    QTreeWidgetItem* addParent(dynamics::Skeleton *skel, QIcon icon);
    QTreeWidgetItem* addChildItem(dynamics::BodyNode* node, QTreeWidgetItem* parent, QIcon icon);
    QTreeWidgetItem* buildTree(dynamics::BodyNode* node, QTreeWidgetItem *prev, QTreeWidgetItem *parent, bool chain);
    void name_Joint(QTreeWidgetItem* node);
    void name_BodyNode(QTreeWidgetItem* node);
};

#endif // TREE_VIEW_H
