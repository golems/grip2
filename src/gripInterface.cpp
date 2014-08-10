#include <QApplication>
#include "GripMainWindow.h"
#include <X11/Xlib.h>
#include "../include/GripInterface.h"
#include <iostream>

GripInterface::GripInterface(){}

GripInterface::~GripInterface(){}

/**
 * \brief Shows the usgae message for grip, by display
 * the command line argument options the user can use
 * \param ostr An ostream operator like std::cout or std::cerr
 */
void GripInterface::show_usage() //(std::ostream &ostr) don't know how to make a python interface to ostream (yet)
{
    // ostr <<
    std::cerr <<
            "Usage: grip <COMMAND> [OPTION]\n"
            "Visualization tool for robotics simulators\n"
            "Commands\n"
            "  -d|--debug                Print debug statements\n"
            "  -f|--file sceneFile       Load scene \"sceneFile\" (.urdf, .sdf)\n"
            "  -c|--config configFile    Load workspace \"configFile\" (.gripconfig)\n"
            "  -h|--help                 Show this help message\n"
            "\n"
            "Examples\n"
            "  grip -d\n"
            "  grip -d -f ~/sceneFiles/robot.urdf\n"
            "  grip --help\n";
}

void GripInterface::sayhello()
{
    std::cerr << "Hello PyGrip!" << std::endl;
}

int GripInterface::step()
{
    std::cerr << "Called step!" << std::endl;
    return 0;
}

int GripInterface::run(int argc, char **argv)
{
    std::cerr << "Running grip instance" << std::endl;

    // Variables for command line parsing
    bool debug = false;
    std::string sceneFilePath;
    std::string configFilePath;

    // Parse command line arguments. See "showUsage" function for description
    std::vector<std::string> args(argv, argv + argc);
    for (size_t i = 1; i < args.size(); ++i) {
        if ("-d" == args[i] || "--debug" == args[i]) {
            debug = true;
        } else if ("-f" == args[i] || "--file" == args[i]) {
            std::cerr << "got f" << std::endl;
            sceneFilePath = args[i+1];
        } else if ("-c" == args[i] || "--config" == args[i]) {
            configFilePath = args[i+1];
        } else if ("-h" == args[i] || "--help" == args[i]) {
            show_usage();
            exit(1);
        }
    }

    // Initialize Xlib support for concurrent threads
    XInitThreads();

    // Start grip
    QApplication app(argc, argv);
    GripMainWindow window(debug, sceneFilePath, configFilePath);
    window.Toolbar();
    window.show();
    return app.exec();
    return 0;
}
