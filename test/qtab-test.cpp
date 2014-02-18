#include <QApplication>
#include "qtab.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    Qtab a;
    a.show();
    return app.exec();
}
