#include <QtGui/QApplication>
#include "carex.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    carex foo;
    foo.show();
    return app.exec();
}
