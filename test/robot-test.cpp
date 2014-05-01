#include <dart/dynamics/Skeleton.h>
//#include <dart/dynamics/BodyNode.h>
//#include <dart/dynamics/Joint.h>
//#include <dart/dynamics/MeshShape.h>
//#include <dart/dynamics/Shape.h>
#include <dart/utils/urdf/DartLoader.h>
//#include <dart/utils/sdf/SdfParser.h>
//#include <dart/simulation/World.h>
//#include <assimp/scene.h>
#include <iostream>
//#include <unistd.h>
//#include "osgAssimpSceneReader.h"
//#include <osg/Group>
//#include <osg/Node>
//#include "osgUtils.h"
//#include <osgViewer/CompositeViewer>
#include "DartNode.h"

using namespace dart;

//void printAiMatrix4x4(aiMatrix4x4 m){
//  std::cerr << "Transformation: " << std::endl;
//  std::cerr << m.a1 << ", " << m.a2 << ", " << m.a3 << ", " << m.a4 << std::endl;
//  std::cerr << m.b1 << ", " << m.b2 << ", " << m.b3 << ", " << m.b4 << std::endl;
//  std::cerr << m.c1 << ", " << m.c2 << ", " << m.c3 << ", " << m.c4 << std::endl;
//  std::cerr << m.d1 << ", " << m.d2 << ", " << m.d3 << ", " << m.d4 << std::endl;
//}

dynamics::Skeleton* getRobot()
{
    // Load robot model from urdf and check if valid
    utils::DartLoader loader;
    dynamics::Skeleton* robot = loader.parseSkeleton("../models/greenBar.urdf");
    if(!robot) {
        std::cerr << "Error parsing robot urdf" << std::endl;
        exit(-1);
    } else {
        std::cerr << "Successfully parsed robot urdf" << std::endl;
        return robot;
    }
}

//void render(osg::Node* root)
//{
//    // Add osg::Group to a view and view to a viewer
//    osgViewer::View* view = osgGolems::createView(0, 0, 800, 800, root);
//    osgViewer::CompositeViewer* viewer = new osgViewer::CompositeViewer();
//    viewer->addView(view);
//    while(!viewer->done()) {
//        viewer->frame();
//    }

//    std::cerr << "Finished" << std::endl;
//}

//int testUrdfParsing()
//{
//    std::cerr << "Starting Robot-Test" << std::endl;

//    utils::DartLoader loader;

//    // Load robot model from urdf and check if valid
//    dynamics::Skeleton* robot = getRobot();

//    // Load world model from urdf and check if valid
//    simulation::World* world = loader.parseWorld("../../../otherRepos/grip-samples/scenes/cubesWorld.urdf");
//    if(!world) {
//        std::cerr << "Error parsing world urdf" << std::endl;
////        return -1;
//    } else {
//        std::cerr << "Successfully parsed world urdf" << std::endl;
//    }

//    // Print robot meta information
//    osg::Group* root = new osg::Group();
//    if(1) {
//        std::cerr << "Name: " << robot->getName() << "\n" << std::endl;
//        for(int i=0; i<robot->getNumBodyNodes(); ++i) {
//            dynamics::BodyNode* node = robot->getBodyNode(i);
//            std::cerr << "BodyNode " << i << ": " << node->getName();
//            if(node->getParentBodyNode()) {
//                std::cerr << "\n\tParent Node: " << node->getParentBodyNode()->getName();
//            }
//            if(node->getParentJoint()) {
//                std::cerr << "\n\tParent Joint: " << node->getParentJoint()->getName();
//                std::cerr << "\n\tLocal TF: \n" << node->getParentJoint()->getLocalTransform().matrix();
//                std::cerr << "\n\tTF from Parent BodyNode: \n" << node->getParentJoint()->getTransformFromParentBodyNode().matrix() << std::endl;
//                std::cerr << "\n\tWorld TF: \n" << node->getWorldTransform().matrix();
//            }
//            for(int j=0; j<node->getNumChildBodyNodes(); ++j) {
//                std::cerr << "\n\tChild Node: " << node->getChildBodyNode(j)->getName();
//            }
//            if(node->getNumVisualizationShapes()) {
//            for(int j=0; j<node->getNumVisualizationShapes(); ++j) {
//                std::cerr << "\t\tj = " << j << std::endl;
//                dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(j);
//                const aiScene* mesh = meshShape->getMesh();
//                std::cerr << "\n\tBodyNode TF: \n" << node->getLocalCOM() << std::endl;
//                osg::Node* n = osgAssimpSceneReader::traverseAIScene(mesh, mesh->mRootNode);
//                root->addChild(n);
//            }
//            }
//            std::cerr << std::endl;
//        }
//    }

//    // Print world meta information
//    if(0) {
//        std::cerr << "Number of skeletons: " << world->getNumSkeletons() << std::endl;
//        for(int i=0; i<world->getNumSkeletons(); ++i) {
//            dynamics::Skeleton* skel = world->getSkeleton(i);
//            std::cerr << "Number of BodyNodes in skeleton " << i << ": " << skel->getNumBodyNodes() << std::endl;
//            for(int j=0; j<skel->getNumBodyNodes(); ++j) {
//                dynamics::BodyNode* node = skel->getBodyNode(j);
//                if(node->getNumVisualizationShapes()) {
//                    for(int k=0; k<node->getNumVisualizationShapes(); ++k) {
//                        dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(k);
//                        const aiScene* mesh = meshShape->getMesh();
//                        osg::Node* n = osgAssimpSceneReader::traverseAIScene(mesh, mesh->mRootNode);
//                        root->addChild(n);
//                    }
//                }
//            }
//        }
//    }

//    render(root);


//    return 0;
//}

//int testDartNode()
//{
//    // Load robot model from urdf and check if valid
//    dynamics::Skeleton* robot = getRobot();

//    osgDart::DartNode* dartNode = new osgDart::DartNode();
//    dartNode->addSkeleton(*robot);

//    std::cerr << "Pre State\n" << robot->getConfig().transpose() << std::endl;
//    std::cerr << "Pre TF\n" << robot->getJoint("LHP")->getTransformFromParentBodyNode().matrix() << std::endl;

//    Eigen::VectorXd qq(1);
//    qq[0] = M_PI/4;

//    std::vector<int> index(1);
//    index[0] = robot->getJoint("LHP")->getGenCoord(0)->getSkeletonIndex();
//    std::cerr << "index " << index[0] << std::endl;
//    robot->setConfig(index, qq);
//    std::cerr << "supposedly LHP: " << robot->getJoint(3)->getName() << std::endl;

//    robot->getBodyNode("Body_LHP")->getChildBodyNode(0)->updateTransform();
//    robot->getBodyNode("Body_LHP")->updateTransform();

//    std::cerr << "Post State\n" << robot->getConfig().transpose() << std::endl;
//    std::cerr << "Post TF\n" << robot->getJoint("LHP")->getLocalTransform().matrix() << std::endl;


//    render(dartNode);
//    return 0;

//}

int testosgDART()
{
  osgDart::DartNode *dartNode = new osgDart::DartNode();
  dart::dynamics::Skeleton *skel = getRobot();
  if (!skel) {
      std::cerr << "NULL Skel" << std::endl;
      return 1;
  } else {
      dartNode->addSkeleton(*skel);
  }
  dartNode->reset();
  delete dartNode;
  return 0;
}

//int testDart()
//{
//    simulation::World* world = new simulation::World();
//    dynamics::Skeleton* robot = getRobot();
//    world->addSkeleton(robot);
//    std::cerr << robot->getName() << "\n"
//              << "\thas " << robot->getNumGenCoords() << " generalized coordinate\n"
//              << "\tLHP: " << robot->getJoint("LHP")->get_q()
//              << std::endl;

//    Eigen::VectorXd q(1);
//    q[0] = M_PI/2;
//    robot->getJoint("LHP")->set_q(q);

//    std::cerr << "LHP: " << robot->getJoint("LHP")->get_q() << std::endl;

//    Eigen::VectorXd config = robot->getState();
//    std::cerr << "Robot config: \n" << config.transpose() << std::endl;

//    return 0;
//}

int main(int argc, char** argv)
{
//    testUrdfParsing();
    // testDartNode();
//    testDart();
  testosgDART();
}
