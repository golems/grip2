
#include "GripObjects.h"


ViewerWidget* GripObjects::getViewerWidget()
{
    return viewWidget;
}

TreeViewReturn* GripObjects::getTreeViewReturn()
{
    return activeItem;
}

std::vector<GripTimeslice>* GripObjects::getTimeline()
{
    return timeline;
}

dart::simulation::World* GripObjects::getWorld()
{
    return world;
}
