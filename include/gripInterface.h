// int run(int argc, char **argv);


#ifndef GRIP_INTERFACE_H
#define GRIP_INTERFACE_H

// C++ Standard includes
#include <iostream>
#include <cstdio>
#include <string>
#include "GripMainWindow.h"

/**
 * \class GripInterface GripInterface.h
 * \brief 
 */
class GripInterface
{
public:
	int some_var;

    GripInterface();

    ~GripInterface();

    /** 
     * Creates a Grip instance
     */
    int create(int argc, char **argv);

    /**
     * \brief Load the scene and renders it. This function resets everything
     * on each load.
     * \param fileName Name of scene file to load
     * \return void
     */
    void loadScene(std::string sceneFileName);

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

    void load(std::string sceneFileName);

    static void show_usage(); //std::ostream &ostr);

protected:
	QApplication * _app;
	GripMainWindow *_window;
	// std::vector<void*> _render_queue();
};

#endif // GRIP_INTERFACE_H
