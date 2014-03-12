#ifndef GRIPTAB_H
#define GRIPTAB_H

#include "TreeViewReturn.h"
#include "../../osgNodes/include/ViewerWidget.h"
#include <QDockWidget>
#include <QtPlugin>

class GripTab : public QDockWidget
{
public:
    TreeViewReturn* activeNode;
    ViewerWidget* viewWidget;

    /// Load active node object from MainWindow
    virtual void LoadActiveNode(TreeViewReturn* ret, ViewerWidget* viewer) = 0;

    /// Fire in relation to dynamic simulation Suitable for controllers
    virtual void GRIPEventSimulationBeforeTimestep() = 0;
    virtual void GRIPEventSimulationAfterTimestep() = 0;
    virtual void GRIPEventSimulationStart() = 0;
    virtual void GRIPEventSimulationStop() = 0;

    /// fires when a new object is selected in the treeview
    virtual void GRIPEventTreeViewSelectionChanged() = 0;
};

Q_DECLARE_INTERFACE(GripTab,
                    "com.gatech.Grip2.GripTab/1.0")

#endif // GRIPTAB_H
