#ifndef GRIPOBJECTS_H
#define GRIPOBJECTS_H

#include "osgGolems/ViewerWidget.h"
#include "qtWidgets/TreeViewReturn.h"
#include "GripTimeslice.h"
#include <dart/simulation/World.h>

class GripObjects
{
public:
    GripObjects() { viewWidget = 0; activeItem = 0; timeline = 0; world = 0; }
    ViewerWidget* getViewerWidget();

    TreeViewReturn* getTreeViewReturn();


    std::vector<GripTimeslice>* getTimeline();

    dart::simulation::World* getWorld();

protected:
    ViewerWidget *viewWidget;
    TreeViewReturn *activeItem;
    std::vector<GripTimeslice> *timeline;
    dart::simulation::World *world;
};

#endif // GRIPOBJECTS_H