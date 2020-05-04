#include "space_emulator.hpp"
#include "input_parameters_form.hpp"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	space_emulator sys(true);

	sys.show();

	return app.exec();
}