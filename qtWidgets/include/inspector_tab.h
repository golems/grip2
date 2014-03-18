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
    void ChangeSelectedJoint(int sliderValue); //, simulation::World* mWorld, Tree_View* treeviewer);
    void ReceiveSeletedItem(TreeViewReturn* active_item);
    void ChangeXPosition(int sliderValue);
    void ChangeYPosition(int sliderValue);
    void ChangeZPosition(int sliderValue);
    void ChangeROrientation(int sliderValue);
    void ChangePOrientation(int sliderValue);
    void ChangeYOrientation(int sliderValue);


private:
    Ui::Inspector_Tab *inspector_ui;
    simulation::World *simworld;
    Tree_View *treeview;
    QSlider *positionSlider_0;
    QDoubleSpinBox *positionSpinBox_0;
};

#endif

#ifndef M_PI
#define M_PI REAL(3.1415926535897932384626433832795029)
#endif
