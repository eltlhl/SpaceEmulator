#include "input_parameters_form.hpp"

input_parameters_form::input_parameters_form(QWidget* parent) noexcept :
	QDialog(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.start_button, &QAbstractButton::pressed, 
		[this]() { close(); });
}
