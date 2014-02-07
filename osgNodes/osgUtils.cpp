
#include "osgUtils.h"
#include <osgViewer/ViewerEventHandlers>

osgViewer::View* createView(int x, int y, int w, int h, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    view->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
    view->setUpViewInWindow(x, y, w, h);
    view->addEventHandler(new osgViewer::StatsHandler);
    if(scene == scene) {
        view->setSceneData(scene);
    }
    return view;
}
