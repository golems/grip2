#include "tree_view.h"
#include <iostream>
#include <cstdio>

// DART includes
#include <dart/utils/urdf/DartLoader.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/simulation/World.h>

using namespace dart;
using namespace std;

Tree_View::Tree_View(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Tree_View)
{
    ui->setupUi(this);
    ui_treeWidget = ui->treeWidget;
    /*
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0, "test");
    ui->treeWidget->addTopLevelItem(itm);
    QTreeWidgetItem *childitm = new QTreeWidgetItem();
    childitm->setText(0, "child test");
    itm->addChild(childitm);
    */
}

Tree_View::~Tree_View()
{
    delete ui;
}

void Tree_View::addParent(QString name)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui_treeWidget);
    itm->setText(0, name);
    ui_treeWidget->addTopLevelItem(itm);
}

void Tree_View::addChild(QString name, QString pname)
{
    QList <QTreeWidgetItem *> list = ui_treeWidget->findItems(pname, Qt::MatchExactly);
    if(! list.isEmpty())
    {
        QTreeWidgetItem *parent = list.front();
        QTreeWidgetItem *childitm = new QTreeWidgetItem();
        childitm->setText(0, name);
        parent->addChild(childitm);
    }
    else
    {
        cout<<"Parent not found in QTreeWidget"<<endl;
    }
}

void Tree_View::populateTreeView(osgDart::DartNode *world, int numRobots)
{
    for (int i = 0; i<= numRobots; i++)
    {
        cout<<"In the tree view populate method "<<numRobots<<endl;
        dynamics::Skeleton* skel = world->getRobot(i);
        addParent(QString::fromStdString(skel->getName()));

        for (int j = 0; j<skel->getNumBodyNodes(); j++)
        {
            addChild(QString::fromStdString(skel->getBodyNode(j)->getName()), QString::fromStdString(skel->getName()));
        }
    }
}
