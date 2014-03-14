#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui_inspector_tab.h"
#include <dart/simulation/World.h>
#include <tree_view.h>

//class ..

class Inspector_Tab : public QDockWidget {
    Q_OBJECT
public:
    Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer);
    ~Inspector_Tab();
private slots:
    void ChangePos0DoubleSpinBox(int sliderValue);
    void ChangePos0Slider(double spinBoxValue);
    void ChangeJoint(int sliderValue); //, simulation::World* mWorld, Tree_View* treeviewer);
    void ReceiveSeletedItem(TreeViewReturn* active_item);

private:
    Ui::Inspector_Tab *inspector_ui;
    simulation::World *simworld;
    Tree_View *treeview;
    QSlider *positionSlider_0;
    QDoubleSpinBox *positionSpinBox_0;
};

#endif

