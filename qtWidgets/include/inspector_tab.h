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
    void changeSelectedJoint(int sliderValue); //, simulation::World* mWorld, Tree_View* treeviewer);
    void receiveSeletedItem(TreeViewReturn* active_item);
    void changePositionAndOrientation(int sliderValue);


private:
    Ui::Inspector_Tab *inspector_ui;
    simulation::World *simworld;
    Tree_View *treeview;
//    QSlider *positionSlider_0;
//    QDoubleSpinBox *positionSpinBox_0;

//    Eigen::Matrix<double, 6, 1> getRootTransform(dart::dynamics::Skeleton* robot);
//    void setRootTransform(dart::dynamics::Skeleton* robot, const Eigen::Matrix<double, 6, 1>& pose);
//    Eigen::Matrix<double, 6, 1> getPoseFromTransform(const Eigen::Isometry3d& tf);
    int selected_type_from_tree;

};

#endif

#ifndef M_PI
#define M_PI REAL(3.1415926535897932384626433832795029)
#endif
