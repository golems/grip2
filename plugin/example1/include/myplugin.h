#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "ui_myplugin.h"
#include <grip/qtWidgets/GripTab.h>
#include <grip/osgGolems/ViewerWidget.h>
#include <grip/qtWidgets/TreeViewReturn.h>


class MyPlugin : public GripTab
{
    Q_OBJECT
    Q_INTERFACES(GripTab)

public:
     MyPlugin(QWidget *parent = 0);
    ~MyPlugin();

    void Load(TreeViewReturn* ret, ViewerWidget* viewer);

    void GRIPEventSimulationBeforeTimestep();
    void GRIPEventSimulationAfterTimestep();
    void GRIPEventSimulationStart();
    void GRIPEventSimulationStop();
    void GRIPEventPlaybackBeforeFrame();
    void GRIPEventPlaybackAfterFrame();
    void GRIPEventPlaybackStart();
    void GRIPEventPlaybackStop();
    void GRIPEventTreeViewSelectionChanged();
    void Refresh();


private:
    Ui::MyPluginTab *ui;

};

#endif // EXAMPLE1_H
