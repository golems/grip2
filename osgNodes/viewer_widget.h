#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

// QT includes
#include <QtCore/QTimer>
#include <QtGui/QGridLayout>

// OpenSceneGraph includes
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include "osgUtils.h"
#include <iostream>

/**
 * \class ViewerWidget, which inherits from QWidget and osgViewer::CompositeViewer.
 * This class creates a osgQt::GraphicsWindowQt and puts in inside a composite viewer
 */
class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    /**
     * \brief Constructor for ViewerWidget class
     */
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded);

    /**
     * \brief Add a osgQt::GraphicsWindowQt widget to the ViewerWidget
     * \param camera An osg::Camera pointer
     * \param scene An osg::Node pointer which is the scene to display
     * \return QWidget pointer
     */
    QWidget* addViewWidget(osg::Camera* camera, osg::Node* scene);

    osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:

    // Timer for update the interface
    QTimer _timer;
};

#endif // VIEWER_WIDGET_H
