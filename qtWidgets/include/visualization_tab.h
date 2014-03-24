#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "ui_visualization_tab.h"
#include "DartNode.h"

//class ..

class Visualization_Tab : public QDockWidget {
    Q_OBJECT
public:
    Visualization_Tab(osgDart::DartNode* worldNode, QWidget *parent = 0);
    ~Visualization_Tab();
    Ui::Visualization_Tab *visualizer_ui;
    void update();

public slots:
    void slotToggleJointAxesVisibility(bool checked);
    void slotToggleBodyNodeAxesVisibility(bool checked);
    void slotToggleSkeletonCoMVisibility(bool checked);
    void slotToggleSkeletonProjCoMVisibility(bool checked);

protected:
    osgDart::DartNode* _worldNode;
};

#endif
