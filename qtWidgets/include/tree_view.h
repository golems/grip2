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
};
Q_DECLARE_METATYPE(TreeViewReturn*);

class Tree_View : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit Tree_View(QWidget *parent = 0, TreeViewReturn* active_item = NULL);
    void populateTreeView(simulation::World* world, int numRobots);
    void clear();
    ~Tree_View();
    TreeViewReturn* getActiveItem();

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
