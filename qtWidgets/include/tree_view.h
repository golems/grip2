#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QDockWidget>
#include "ui_tree_view.h"
#include <dart/simulation/World.h>

using namespace dart;

namespace Ui {
class Tree_View;
}

class Tree_View : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit Tree_View(QWidget *parent = 0);
    void populateTreeView(simulation::World* world, int numRobots);
    ~Tree_View();
    
private:
    Ui::Tree_View *ui;
    QTreeWidget *ui_treeWidget;

    void addParent(QString name);
    void addChild(QString name, QString pname);

};

#endif // TREE_VIEW_H
