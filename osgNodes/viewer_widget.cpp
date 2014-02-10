#include "viewer_widget.h"
#include <osg/io_utils>
#include "osgUtils.h"

osg::Node* getSceneData()
{
    osg::Group* root = new osg::Group();
    root->setDataVariance(osg::Object::DYNAMIC);
    osg::Node* robot = osgDB::readNodeFile("../grip2/data/robot.osg");
    root->addChild(robot);
    return root;
}

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
    setThreadingModel(threadingModel);

    osg::Node* sceneData = getSceneData();

    QWidget* widget1 = addViewWidget(createCamera(0,0,100,100), sceneData);
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
    setLayout(grid);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start( 10 );
}

QWidget* ViewerWidget::addViewWidget(osg::Camera* camera, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera(camera);
    addView(view);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);

    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());
    return gw ? gw->getGLWidget() : NULL;
}

osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
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
    camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f);
    return camera.release();
}

osg::Matrixd ViewerWidget::getViewMatrix()
{
    osg::Matrixd m = getView(0)->getCameraManipulator()->getMatrix();
    return m;
}

void ViewerWidget::setViewMatrix(uint i, osg::Matrixd m)
{
    uint numViews = this->getNumViews();
    if(i < numViews) {
        this->getView(i)->getCameraManipulator()->setByMatrix(m);
    } else {
        std::cerr << "Error. You tried to change view " << i << ", but there are only "
                  << numViews << " views" << std::endl;
    }
}

void ViewerWidget::addNodeToScene(osg::Node* node, uint viewNum)
{
    osg::Group* scene = this->getView(viewNum)->getSceneData()->asGroup();
    if(scene != scene) {
        std::cerr << "Error! Can not convert from osg::Node to osg::Group."
                  << std::endl;
    } else {
        scene->addChild(node);
    }
}

void ViewerWidget::addCenterAxes()
{
    osg::Node* axes = createNode();
    addNodeToScene(axes);
}
