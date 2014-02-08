#include "viewer_widget.h"
#include "QApplication"
#include <osg/io_utils>

int main( int argc, char** argv )
{
    QApplication app(argc, argv);
    ViewerWidget* viewWidget = new ViewerWidget();
    viewWidget->setGeometry(100, 100, 800, 600);
//    osgViewer::View* cameraView = createView(1000, 150, 450, 450, osgDB::readNodeFile("../grip2/data/robot.osg"));
//    viewWidget->addView(cameraView);
//    std::cerr << "viewMatrix init:\n" << viewWidget->getViewMatrix() << std::endl;
    viewWidget->show();
    return app.exec();
}
