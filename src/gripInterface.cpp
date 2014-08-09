#include <QApplication>
#include "GripMainWindow.h"
#include <X11/Xlib.h>
#include "../include/gripInterface.h"
#include <iostream>

int run(int argc, char **argv)
{
    std::cerr << "Hello Grip" << std::endl;
    // Initialize Xlib support for concurrent threads
    XInitThreads();

    // Start grip
    QApplication app(argc, argv);
    GripMainWindow window;
    window.Toolbar();
    window.show();
    return app.exec();
    return 0;
}
