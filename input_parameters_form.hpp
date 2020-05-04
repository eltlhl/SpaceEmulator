#pragma once

#include <QWidget>

#include "ui_input_parameters_form.h"

class input_parameters_form : public QWidget
{
	Q_OBJECT

public:
	input_parameters_form(QWidget *parent = Q_NULLPTR);
	inline ~input_parameters_form() noexcept { };

private:
	Ui::input_parameters_form m_ui;
};
