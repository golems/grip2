// int run(int argc, char **argv);


#ifndef GRIP_INTERFACE_H
#define GRIP_INTERFACE_H

// C++ Standard includes
#include <iostream>
#include <cstdio>

/**
 * \class MainWindow MainWindow.h
 * \brief Main window for robot simulation interface. A virtual base class
 * that subclasses QMainWindow
 */
class GripInterface
{
public:
	int some_var;

    GripInterface();

    ~GripInterface();

    /** 
     * Renders the current scene
     */
    int run(int argc, char **argv);

    /** 
     * Steps the world
     * TODO: implement and bind to dart
     */
    int step();

    /** 
     * Renders the current scene and executes all drawing callbacks
     * in the render queue.
     */
    void render();

    static void show_usage(); //std::ostream &ostr);

    static void sayhello();


protected:
	// std::vector<void*> _render_queue();
};

#endif // GRIP_INTERFACE_H
