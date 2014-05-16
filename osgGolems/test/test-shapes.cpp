#include "../osgUtils.h"
#include "../GripCameraManipulator.h"
#include "../Shapes.h"
#include "../Axes.h"
#include "../Grid.h"

#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <Eigen/Geometry>

int main(int argc, char **argv)
{
/*GRID*/
    // Create 5m x 5m gray grid with 1m x 1m grid squares
    // Turn off lighting so it's always the color we want no matter
    // which angle we view it from.
    osg::Geode* gridGeode = new osg::Geode();
    osgGolems::Grid* grid = new osgGolems::Grid(5, 5, 1, osg::Vec4(.3, .3, .3, .5));
    gridGeode->addDrawable(grid);
    osgGolems::setStateMode(gridGeode, GL_LIGHTING, osg::StateAttribute::OFF
                            | osg::StateAttribute::PROTECTED);

/*AXES*/
    // Add RGB axes to center of the scene at (0,0,0).
    // Line width is relative, not absolute.
    osg::ref_ptr<osgGolems::Axes> axes = new osgGolems::Axes(.3);
    axes->setLineWidth(3);
    osg::Geode* axesGeode = new osg::Geode;
    axesGeode->addDrawable(axes);
    osgGolems::setStateMode(axesGeode, GL_LIGHTING, osg::StateAttribute::OFF
                            | osg::StateAttribute::PROTECTED);

/*SPHERE*/
    // Create semi-transparent red sphere by itself at x=1, y=z=0;
    // It's not put inside a MatrixTransform, which means we can't change its
    // world transform, only it's position (ie. not its orientation)
    osg::ref_ptr<osgGolems::Sphere> sphere =
            new osgGolems::Sphere(osg::Vec3(1,0,0), 0.4, osg::Vec4(1,0,0,0.4));

/*CYLINDER*/
    // Create semi-transparent yellow cylinder and put it inside a Matrix Transform so that
    // we can put it horizontal tangent to the sphere on the -y side.
    // We set the state attribute "MATERIAL" to ON and PROTECTED.
    // "ON" means GL_MATERIAL mode is enabled
    // "PROTECTED" means its parent node can't override it's material settings
    osg::ref_ptr<osgGolems::Cylinder> cylinder = new osgGolems::Cylinder;
    cylinder->setRadius(0.2);
    cylinder->setHeight(0.5);
    cylinder->setColor(osg::Vec4(1,1,0,.4));
    osgGolems::setStateAttribute(cylinder, osg::StateAttribute::MATERIAL,
                osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    osg::ref_ptr<osg::MatrixTransform> cylinderTF = new osg::MatrixTransform;
    Eigen::Isometry3f cylinderMat = Eigen::Isometry3f::Identity();
    Eigen::Vector3f cylTrans(sphere->getCenter().x(),
                             -(sphere->getRadius() + (cylinder->getHeight() / 2)),
                             (float)0);
    cylinderMat.translate(cylTrans);
    cylinderMat.rotate(Eigen::AngleAxisf(M_PI/2, Eigen::Vector3f::UnitX()));
    cylinderTF->setMatrix(osgGolems::eigToOsgMatrix(cylinderMat));
    cylinderTF->addChild(cylinder);

/*CONE*/
    // Create opaque green cone and place it on top of cylinder inside
    // the cylinderTF matrix transform so that it gets transformed
    // as well.
    osg::ref_ptr<osgGolems::Cone> cone = new osgGolems::Cone;
    cone->setRadius(cylinder->getRadius()+.1);
    cone->setColor(osg::Vec4(0, 1, 0, 1));
    cone->setHeight(cylinder->getRadius() * 2.0);
    osg::ref_ptr<osg::MatrixTransform> coneTF = new osg::MatrixTransform;
    osg::Matrix coneMat;
    coneMat.makeTranslate(0, 0, (cylinder->getHeight() / 2.0)
                          + (cylinder->getRadius() * 2.0 / 4.0));
    coneTF->setMatrix(coneMat);
    coneTF->addChild(cone);
    cylinderTF->addChild(coneTF);

/*ROOT*/
    // Create a root group and
    // add the shapes and shape TFs to the root group.
    osg::ref_ptr<osg::Group> sceneData = new osg::Group;
    sceneData->addChild(gridGeode);
    sceneData->addChild(axesGeode);
    sceneData->addChild(sphere);
    sceneData->addChild(cylinderTF);

/*VIEWER*/
    // Create viewer of size 700 x 900 pixels with the top-left
    // corner at (50, 10).
    // Add the root scene data group to the viewer and visualize it.
    // For grip you would just call the following function itstead
    // _viewer->addNodeToScene(sceneData);
    osgViewer::Viewer *viewer = new osgViewer::Viewer;
    viewer->setUpViewInWindow(50,10,700,900);
    viewer->setSceneData(sceneData);
    viewer->setCameraManipulator(new osgGolems::GripCameraManipulator);
    while(!viewer->done()) {
        viewer->frame();
    }
    return 0;
}
