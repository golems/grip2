#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>

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
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded) : QWidget()
    {
        setThreadingModel(threadingModel);

        QWidget* widget1 = addViewWidget( createCamera(0,0,100,100), osgDB::readNodeFile("../grip2/data/robot.osg") );
//        QWidget* widget2 = addViewWidget( createCamera(0,0,50,50), osgDB::readNodeFile("../grip2/data/robot.osg") );

        QGridLayout* grid = new QGridLayout;
        grid->addWidget( widget1, 0, 0 );
//        grid->addWidget( widget2, 1, 0 );
        setLayout( grid );
        connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
        _timer.start( 10 );
    }
    
    QWidget* addViewWidget( osg::Camera* camera, osg::Node* scene )
    {
        osgViewer::View* view = new osgViewer::View;
        view->setCamera(camera);
        addView(view);
        
        view->setSceneData( scene );
        view->addEventHandler( new osgViewer::StatsHandler );
        osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
        cameraManipulator->setAllowThrow(false);
        view->setCameraManipulator(cameraManipulator);

        osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
        return gw ? gw->getGLWidget() : NULL;
    }
    
    osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false )
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowName = name;
        traits->windowDecoration = windowDecoration;
        traits->x = x;
        traits->y = y;
        traits->width = w;
        traits->height = h;
        traits->doubleBuffer = true;
        traits->alpha = ds->getMinimumNumAlphaBits();
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();
        
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
        
        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        camera->setProjectionMatrixAsPerspective(
            30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
        return camera.release();
    }
    
    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:
    
    QTimer _timer;
};

osgViewer::Viewer* createViewer(int x, int y, int w, int h, osg::Node* scene)
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    viewer->setSceneData(scene);
    viewer->setUpViewInWindow(x, y, w, h);

    // Create camera with no inertia and add it to the viewer.
    osgGA::OrbitManipulator* camera = new osgGA::OrbitManipulator();
    camera->setAllowThrow(false);
    viewer->setCameraManipulator(camera);
    viewer->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
    viewer->realize();
}

osgViewer::View* createView(int x, int y, int w, int h, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    view->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
    view->setUpViewInWindow(x, y, w, h);
    view->setSceneData(scene);
    return view;
}

int main( int argc, char** argv )
{
    osg::ArgumentParser arguments(&argc, argv);

    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
    while (arguments.read("--SingleThreaded")) threadingModel = osgViewer::ViewerBase::SingleThreaded;
    while (arguments.read("--CullDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
    while (arguments.read("--DrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;
    
    QApplication app(argc, argv);
    ViewerWidget* viewWidget = new ViewerWidget(threadingModel);
    viewWidget->setGeometry( 100, 100, 800, 600 );
//    osgViewer::View* cameraView = createView(0, 0, 50, 50, osgDB::readNodeFile("../grip2/data/robot.osg"));
//    viewWidget->addView(cameraView);


    viewWidget->show();
    return app.exec();
}
