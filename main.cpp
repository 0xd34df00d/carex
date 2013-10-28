#include <QApplication>
#include "carex.h"

int main (int argc, char** argv)
{
	QApplication app (argc, argv);
	Carex c;
	c.show ();
	return app.exec ();
}
