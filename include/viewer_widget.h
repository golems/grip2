// QT includes
#include <QtCore/QTimer>
#include <QtGui/QGridLayout>

// OpenSceneGraph includes
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include <stdlib.h>
#include <iostream>


class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded);

    QWidget* addViewWidget(osg::Camera* camera, osg::Node* scene);

    osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:

    QTimer _timer;
};
