#include "../include/GripInterface.h"

#include <QApplication>
#include <X11/Xlib.h>
#include <iostream>
#include <unistd.h>
#include <Eigen/Geometry>

#include "GripTools.h"

#if defined(__linux) || defined(__linux__) || defined(linux)
    // anything?
#elif defined(__APPLE__)
    #include <dispatch/dispatch.h>
#endif

GripInterface::GripInterface() : _app(NULL), _window(NULL), _gripthread(NULL)
{
}

GripInterface::~GripInterface()
{
    // kill grip somehow?
    // _window->exit();
    // _app->quit();
    // std::cerr << "window should have exited" << std::endl;    
    if (_gripthread != NULL)
        _gripthread->join();
}

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

int GripInterface::_create(int argc, char **argv)
{
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
    _app = new QApplication(argc, argv);
    _window = new GripMainWindow(debug, sceneFilePath, configFilePath);
    _window->Toolbar();
    _window->show();
    _app->exec();
    
    return 0;
}

#if defined(__linux) || defined(__linux__) || defined(linux)
/**
 * Runs Grip QT application in a thread -- linux version
 */
int GripInterface::run(int argc, char **argv)
{
    // std::cerr << "trying to run grip in a thread" << "(this=" << this << ")" << std::endl;
    _gripthread = new std::thread(&GripInterface::_create, this, argc, argv);

    // don't return until window is created
    while (_window == NULL)
        usleep(1000);

    return 0;
}

#elif defined(__APPLE__)
/**
 * Runs Grip QT application in a thread -- mac version.
 * This approach was supposed to work but sadly causes segfaults
 * on my mac.  Haven't found a workaround yet, so for now only
 * the linux version supports threaded execution.  
 * The problem is with QTApp's exec call, which expects to be 
 * top dog.  We can bring up the window without calling this, 
 * but it won't be interactive.
 */
int GripInterface::run(int argc, char **argv)
{
    /*
     * Get the main serial queue.
     * It doesn't start processing until we call dispatch_main()
     */

    /* Desired solution using OSX dispatch queue */
    // dispatch_queue_t main_q = dispatch_get_main_queue();
    // dispatch_async(main_q, ^{
    //     std::cerr << "trying to create grip now" << std::endl;
    //     _create(argc, argv); // Fails for some reason
    // });
    // /* Start the main queue */
    // dispatch_main();

     /* Temporary solution */
     _create(argc, argv); // uncomment to just run in main thread (blocks)

    return 0;
}
#endif

void GripInterface::loadScene(std::string sceneFileName)
{
    if (_window == NULL)
        std::cerr << "Grip window pointer is NULL.  Call create()." << std::endl;
    else
        _window->doLoad(sceneFileName);
}

void GripInterface::loadPluginFile(std::string pluginFileName)
{
    _window->loadPluginFile(QString::fromStdString(pluginFileName));
}

void GripInterface::render()
{
    _window->viewWidget->update();
}

void GripInterface::startSimulation()
{
    _window->startSimulation();
}

void GripInterface::stopSimulation()
{
    _window->stopSimulation();
}

void GripInterface::simulateSingleStep()
{
    _window->simulateSingleStep();
}

std::vector<double> GripInterface::getState()
{
    /* Note: would be nice to memory map this, but stl vector 
       doesn't seem to support it.
    */ 
    // Eigen::Map<Eigen::VectorXd>(state.data(), _estate.rows(), _estate.cols()) = _estate;

  std::vector<double> state;
  Eigen::VectorXd worldState;
  worldState = getWorldState( _window->world );
  
  for( unsigned int j = 0; j < worldState.size(); ++j ) {
    state.push_back( worldState(j) );
  }
  return state;
}

void GripInterface::setState(const std::vector<double> &state)
{
  Eigen::Map<const Eigen::VectorXd> _es(state.data(), state.size());
  setWorldState( _window->world, _es );
}
