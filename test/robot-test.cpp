#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Joint.h>
#include <dart/utils/urdf/DartLoader.h>
#include <dart/utils/sdf/SdfParser.h>
#include <dart/simulation/World.h>
#include <iostream>
#include <unistd.h>

using namespace dart;

int main(int argc, char** argv)
{
    std::cerr << "Starting Robot-Test" << std::endl;
    char dir[100];
    getcwd(dir, 100);
    std::cerr << "dir: " << dir << std::endl;
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
        std::cerr << std::endl;
    }

    return 0;
}
