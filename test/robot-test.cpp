#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/MeshShape.h>
#include <dart/dynamics/Shape.h>
#include <dart/utils/urdf/DartLoader.h>
#include <dart/utils/sdf/SdfParser.h>
#include <dart/simulation/World.h>
#include <assimp/scene.h>
#include <iostream>
#include <unistd.h>
#include "osgAssimpSceneReader.h"
#include <osg/Group>
#include <osg/Node>
#include "osgUtils.h"
#include <osgViewer/CompositeViewer>

using namespace dart;

int main(int argc, char** argv)
{
    std::cerr << "Starting Robot-Test" << std::endl;

    // Load urdf model of the robot
    utils::DartLoader loader;
    dynamics::Skeleton* robot;
    robot = loader.parseSkeleton("../models/drchubo_v2/robots/drchubo_v2.urdf");

    // Check if it loaded correctly
    if(!robot) {
        std::cerr << "Error parsing urdf" << std::endl;
        return -1;
    } else {
        std::cerr << "Successfully parsed urdf" << std::endl;
    }

    // Print robot meta information
    osgAssimpSceneReader assimpReader;

    osg::Group* root = new osg::Group();
    std::cerr << "Name: " << robot->getName() << "\n" << std::endl;
    if(1) {
        for(int i=0; i<robot->getNumBodyNodes(); ++i) {
            dynamics::BodyNode* node = robot->getBodyNode(i);
            std::cerr << "BodyNode " << i << ": " << node->getName();
            if(node->getParentBodyNode()) {
                std::cerr << "\n\tParent Node: " << node->getParentBodyNode()->getName();
            }
            if(node->getParentJoint()) {
                std::cerr << "\n\tParent Joint: " << node->getParentJoint()->getName();
            }
            for(int j=0; j<node->getNumChildBodyNodes(); ++j) {
                std::cerr << "\n\tChild Node: " << node->getChildBodyNode(j)->getName();
            }
            if(node->getNumVisualizationShapes()) {
            for(int j=0; j<node->getNumVisualizationShapes(); ++j) {
                dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(j);
                const aiScene* mesh = meshShape->getMesh();
                std::cerr << "\n\tnum verts: " << mesh->mMeshes[j]->mNumVertices << std::endl;
                osg::Node* n = assimpReader.traverseAIScene(mesh, mesh->mRootNode);
                root->addChild(n);
            }
            }
            std::cerr << std::endl;
        }
    }

    osgViewer::View* view = createView(0, 0, 500, 500, root);
    view->setUpViewOnSingleScreen(0);
    osgViewer::CompositeViewer* viewer = new osgViewer::CompositeViewer();
    viewer->addView(view);
    while(!viewer->done()) {
        viewer->frame();
    }


//    if(true) {
//        dynamics::BodyNode* rootNode = robot->getRootBodyNode();
//        if(->getNumChildBodyNodes())
//        for(int i=0; i<robot->getRootBodyNode(); ++i) {
//            dynamics::BodyNode* node = robot->getBodyNode(i);
//            std::cerr << "BodyNode " << i << ": " << node->getName();
//            if(node->getParentBodyNode()) {
//                         std::cerr << "\n\tParent Node: " << node->getParentBodyNode()->getName();
//            }
//            if(node->getParentJoint()) {
//                         std::cerr << "\n\tParent Joint: " << node->getParentJoint()->getName();
//            }
//            for(int j=0; j<node->getNumChildBodyNodes(); ++j) {
//                         std::cerr << "\n\tChild Node: " << node->getChildBodyNode(j)->getName();
//            }
//            std::cerr << std::endl;
//        }
//    }

    std::cerr << "\n\n Finished \n\n";

    return 0;
}
