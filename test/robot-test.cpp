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

    utils::DartLoader loader;

    // Load robot model from urdf and check if valid
    dynamics::Skeleton* robot = loader.parseSkeleton("../models/drchubo_v2/robots/drchubo_v2.urdf");
    if(!robot) {
        std::cerr << "Error parsing robot urdf" << std::endl;
        return -1;
    } else {
        std::cerr << "Successfully parsed robot urdf" << std::endl;
    }

    // Load world model from urdf and check if valid
    simulation::World* world = loader.parseWorld("../../../otherRepos/grip-samples/scenes/cubesWorld.urdf");
    if(!world) {
        std::cerr << "Error parsing world urdf" << std::endl;
        return -1;
    } else {
        std::cerr << "Successfully parsed world urdf" << std::endl;
    }

    // Print robot meta information
    osg::Group* root = new osg::Group();
    if(0) {
        std::cerr << "Name: " << robot->getName() << "\n" << std::endl;
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
                std::cerr << "\t\tj = " << j << std::endl;
                dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(j);
                const aiScene* mesh = meshShape->getMesh();
                std::cerr << "\n\tnum verts: " << mesh->mMeshes[j]->mNumVertices << std::endl;
                osg::Node* n = osgAssimpSceneReader::traverseAIScene(mesh, mesh->mRootNode);
                root->addChild(n);
            }
            }
            std::cerr << std::endl;
        }
    }

    // Print world meta information
    if(1) {
        std::cerr << "Number of skeletons: " << world->getNumSkeletons() << std::endl;
        for(int i=0; i<world->getNumSkeletons(); ++i) {
            dynamics::Skeleton* skel = world->getSkeleton(i);
            std::cerr << "Number of BodyNodes in skeleton " << i << ": " << skel->getNumBodyNodes() << std::endl;
            for(int j=0; j<skel->getNumBodyNodes(); ++j) {
                dynamics::BodyNode* node = skel->getBodyNode(j);
                if(node->getNumVisualizationShapes()) {
                    for(int k=0; k<node->getNumVisualizationShapes(); ++k) {
                        dynamics::MeshShape* meshShape = (dynamics::MeshShape*)node->getVisualizationShape(k);
                        const aiScene* mesh = meshShape->getMesh();
                        osg::Node* n = osgAssimpSceneReader::traverseAIScene(mesh, mesh->mRootNode);
                        root->addChild(n);
                    }
                }
            }
        }
    }

    // Add osg::Group to a view and view to a viewer
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
