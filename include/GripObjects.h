#ifndef GRIPOBJECTS_H
#define GRIPOBJECTS_H

#include "ViewerWidget.h"
#include "TreeViewReturn.h"
#include "../include/GripTimeslice.h"
#include <dart/simulation/World.h>

class GripObjects
{
public:
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