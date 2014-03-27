#include <QtGui>
#include "TreeView.h"
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

TreeView::TreeView(QWidget *parent, TreeViewReturn* active_item) :QDockWidget(parent), ui(new Ui::TreeView)
{
    activeItem = 0;
    ui->setupUi(this);
    ui_treeWidget = ui->treeWidget;
    ui_treeWidget->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui_treeWidget->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui_treeWidget->header()->setStretchLastSection(false);
    connect(ui_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeView_itemSelected(QTreeWidgetItem*,int)));

    ui_checkBox = ui->checkBox;
    connect(ui_checkBox, SIGNAL(stateChanged(int)), this, SLOT(nameChange_BodyNode_Joint(int)));
}

TreeView::~TreeView()
{
    delete ui;
}

void TreeView::treeView_itemSelected(QTreeWidgetItem * item, int column)
{
    TreeViewReturn* val = item->data(0, Qt::UserRole).value<TreeViewReturn*>();
    activeItem = val;
    emit itemSelected(activeItem);
}

TreeViewReturn* TreeView::getActiveItem()
{
    return activeItem;
}

QTreeWidgetItem* TreeView::addParent(dynamics::Skeleton* skel, QIcon icon, int skeleton_id)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui_treeWidget);
    itm->setText(0, QString::fromStdString(skel->getName()));
    itm->setIcon(0, icon);

    TreeViewReturn* ret = new TreeViewReturn();
    ret->object = skel;
    ret->dType = Return_Type_Robot;
    ret->skeletonID = skeleton_id;

    QVariant var;
    var.setValue(ret);
    itm->setData(0, Qt::UserRole, var);

    ui_treeWidget->addTopLevelItem(itm);
    return itm;
}

QTreeWidgetItem* TreeView::addChildItem(dynamics::BodyNode* node, QTreeWidgetItem* parent, QIcon icon, int skeleton_id)
{
    if(parent != NULL)
    {
        QTreeWidgetItem *childitm = new QTreeWidgetItem();
        childitm->setText(0, QString::fromStdString(node->getName()));
        childitm->setIcon(0,icon);

        TreeViewReturn* ret = new TreeViewReturn();
        ret->object = node;
        ret->dType = Return_Type_Node;
        ret->skeletonID = skeleton_id;

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

QTreeWidgetItem* TreeView::buildTree(dynamics::BodyNode* node, QTreeWidgetItem* prev, QTreeWidgetItem* parent, bool chain, int skeleton_id)
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
            prev = new_parent = addChildItem(node, parent, QIcon(icon), skeleton_id);
        else
        {
            prev = addChildItem(node, parent, QIcon(icon), skeleton_id);
        }
        buildTree(node->getChildBodyNode(0), prev, new_parent, true, skeleton_id);
    }
    else
    {
        prev = new_parent = addChildItem(node, parent, QIcon(icon), skeleton_id);
        for (int i=0; i<node->getNumChildBodyNodes(); i++)
            prev = buildTree(node->getChildBodyNode(i), prev, new_parent, false, skeleton_id);
    }
    return prev;
}

void TreeView::populateTreeView(simulation::World *world)
{
    QPixmap robotIcon((const char**) robot_xpm);
    for (int i = 0; i<world->getNumSkeletons(); ++i)
    {
        dynamics::Skeleton* skel = world->getSkeleton(i);
        if(skel) {
            QTreeWidgetItem* parent = addParent(skel,  QIcon(robotIcon), i);
            buildTree(skel->getRootBodyNode(), parent, parent, false,i);
        } else {
            std::cerr << "Not a valid skeleton. Not building tree view. (Line " << __LINE__ << " of " << __FILE__ << std::endl;
        }
    }
}

void TreeView::clear()
{
    while(ui_treeWidget->topLevelItemCount()) {
        delete ui_treeWidget->takeTopLevelItem(0);
    }
}

void TreeView::name_Joint(QTreeWidgetItem* node)
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

void TreeView::name_BodyNode(QTreeWidgetItem* node)
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

void TreeView::nameChange_BodyNode_Joint(int checkBoxState)
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