#include <QApplication>
//#include "gripmainwindow.h"
#include "mainwindow.h"

#include <X11/Xlib.h>

int main(int argc, char *argv[])
{
    XInitThreads();
	QApplication app(argc, argv);
   // GripMainWindow window;
    MainWindow window;
 
    window.Toolbar();
    window.show();
    return app.exec();
}
