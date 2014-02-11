#include "viewer_widget.h"
#include <osg/io_utils>
#include "osgUtils.h"
#include "Axes.h"
#include "CameraCallback.h"

osg::Node* ViewerWidget::getSceneData()
{
    // Add root
    osg::Group* root = new osg::Group();

    // Add robot
    osg::Node* robot = osgDB::readNodeFile("../grip2/data/robot.osg");
    osg::MatrixTransform* tf = new osg::MatrixTransform();
    tf->addChild(robot);
    std::cerr << "tf:\n" << tf->getMatrix() << std::endl;
    root->addChild(tf);

    // Axes for center of rotation
    osg::MatrixTransform* axesTF = new osg::MatrixTransform();
    axesTF->setDataVariance(osg::Object::DYNAMIC);
    CameraData* cameraData = new CameraData;
//    axesTF->setUserData(this->getView(0)->getCamera());
    axesTF->setUserData(cameraData);
    osg::Geode* axesGeode = new osg::Geode();
    Axes* axes = new Axes(2);
    axesGeode->addDrawable(axes);
    axesTF->addChild(axesGeode);
    root->addChild(axesTF);

    return root;
}

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
    setThreadingModel(threadingModel);

    // Create scene data
    osg::Node* sceneData = getSceneData();

    // Create view widget with camera and scene data
    QWidget* widget1 = addViewWidget(createCamera(0,0,100,100), sceneData);

    // Create QLayout and add view widget to it. Then set it as the main layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(widget1);
    setLayout(layout);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

QWidget* ViewerWidget::addViewWidget(osg::Camera* camera, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera(camera);
    this->addView(view);

    view->setSceneData(scene);

    view->addEventHandler(new osgViewer::StatsHandler);

    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    std::cerr << "Center: " << cameraManipulator->getCenter() << std::endl;

    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());
    return gw ? gw->getGLWidget() : NULL;
}

osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
{
    // Get display settings instance in order to set our traits
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

    // Create GraphicsContext traits and set them.
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
        osg::ref_ptr<osgGA::OrbitManipulator> c =
            dynamic_cast<osgGA::OrbitManipulator*>(this->getView(0)->getCameraManipulator());
        c->setCenter(osg::Vec3f(0, 0, 0));
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

void ViewerWidget::setBackgroundColor(const osg::Vec4 &color, uint viewNum)
{
    if(viewNumIsValid(viewNum)) {
        this->getView(viewNum)->getCamera()->setClearColor(color);
    }
}

bool ViewerWidget::viewNumIsValid(uint viewNum)
{
    if(viewNum >= 0 && viewNum < this->getNumViews()) {
        return true;
    } else {
        fprintf(stderr, "Error! Requested view #%d, but only %d views exist\n", viewNum, this->getNumViews());
        fprintf(stderr, "Valid view #'s are 0 - %d\n", this->getNumViews() - 1);
        return false;
    }
}
