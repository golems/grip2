#include <QApplication>
#include "GripMainWindow.h"
#include <X11/Xlib.h>

int main(int argc, char *argv[])
{
    XInitThreads();
	QApplication app(argc, argv);
    GripMainWindow window(true);
    window.Toolbar();
    window.show();
    return app.exec();
}
