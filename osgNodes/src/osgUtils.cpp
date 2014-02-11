
#include "osgUtils.h"
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include "Axes.h"

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

void addNodeToScene(osg::Group* scene)
{
    scene->addChild(createNode());
}

osg::Node* createNode()
{
    //                 --- tf1 --- geode --- cylinder
    // root --- rootTF |
    //                 --- tf2 --- geode2 --- cylinderClone

//    osg::Group* root = new osg::Group();
    // Create cylinder
    osg::Geode* geode = new osg::Geode;
    osg::TessellationHints* hints = new osg::TessellationHints;
    osg::ShapeDrawable* cylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0.0f), .1, 1), hints);
    cylinder->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    geode->addDrawable(cylinder);
//    osg::Geode* geode2 = new osg::Geode;
//    osg::ShapeDrawable* cylinder2 = clone(cylinder);
//    cylinder2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//    geode2->addDrawable(cylinder2);

    // Create dynamics TF
    osg::MatrixTransform* rootTF = new osg::MatrixTransform;
    osg::MatrixTransform* tf1 = new osg::MatrixTransform;
//    osg::MatrixTransform* tf2 = new osg::MatrixTransform;
    tf1->setDataVariance(osg::Object::DYNAMIC);
//    tf1->setUpdateCallback(new LineCallback);
//    tf2->setDataVariance(osg::Object::DYNAMIC);
//    tf2->setUpdateCallback(new LineCallback);
    osg::Matrix m;
    m.makeTranslate(3, 0, 0);
    tf1->setMatrix(m);
    osg::Geode* g = new osg::Geode;
    Axes* a = new Axes();
    g->addDrawable(a);
    tf1->addChild(g);
//    tf2->addChild(geode2);

    // Add TF to root
    rootTF->addChild(tf1);
    //rootTF->addChild(tf2);
    //root->addChild(rootTF);

    // switch off lighting as we haven't assigned any normals.
//    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    return rootTF;
}
