#include "space_emulator.hpp"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	space_emulator se(true);
	se.show();

	return app.exec();
}