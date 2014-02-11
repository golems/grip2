#include "viewer_widget.h"
#include "QApplication"
#include <osg/io_utils>

//int callbackTest()
//{
//    osg::Group* root = new osg::Group();

//    // Create cylinder
//    osg::Geode* geode = new osg::Geode;
//    osg::TessellationHints* hints = new osg::TessellationHints;
//    osg::ShapeDrawable* cylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0), .01, 1), hints);
//    cylinder->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
//    geode->addDrawable(cylinder);
//    osg::Geode* geode2 = new osg::Geode;
//    osg::ShapeDrawable* cylinder2 = clone(cylinder);
//    cylinder2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//    geode2->addDrawable(cylinder2);

//    // Create dynamics TF
//    osg::MatrixTransform* rootTF = new osg::MatrixTransform;
//    osg::MatrixTransform* tf1 = new osg::MatrixTransform;
//    osg::MatrixTransform* tf2 = new osg::MatrixTransform;
//    tf1->setDataVariance(osg::Object::DYNAMIC);
//    tf1->setUpdateCallback(new LineCallback);
//    tf2->setDataVariance(osg::Object::DYNAMIC);
////    tf2->setUpdateCallback(new LineCallback);
//    osg::Matrix m;
//    m.makeTranslate(0.6, 0, 0);
//    tf1->setMatrix(m);
//    tf1->addChild(geode);
//    tf2->addChild(geode2);

//    // Add TF to root
//    rootTF->addChild(tf1);
//    rootTF->addChild(tf2);
//    root->addChild(rootTF);

//    // switch off lighting as we haven't assigned any normals.
//    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

//    osgViewer::CompositeViewer* viewer = new osgViewer::CompositeViewer();
//    osgViewer::View* view = createView(0, 0, 640, 480, root);
//    view->setUpViewOnSingleScreen(0);
//    viewer->addView(view);

//    while(!viewer->done()) {
//        viewer->frame();
//    }

//    return 0;
//}

int main( int argc, char** argv )
{
    QApplication app(argc, argv);
    ViewerWidget* viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
//    osgViewer::View* cameraView = createView(1000, 150, 450, 450, osgDB::readNodeFile("../grip2/data/robot.osg"));
//    viewWidget->addView(cameraView);
//    std::cerr << "viewMatrix init:\n" << viewWidget->getViewMatrix() << std::endl;
//    viewWidget->getView(0)->getCameraManipulator()->getInverseMatrix();
    viewWidget->show();
    return app.exec();
    osgViewer::Viewer* v = new osgViewer::Viewer();
}
