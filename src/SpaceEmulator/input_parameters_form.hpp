#pragma once

#include <QDialog>

#include <tuple>

#include "ui_input_parameters_form.h"

class input_parameters_form : public QDialog
{
	Q_OBJECT

public:
	using params_t = std::tuple<double, double, double>;

public:
	input_parameters_form(QWidget* parent) noexcept;
	inline ~input_parameters_form() noexcept = default;

	[[nodiscard]]
	__forceinline params_t get_values() const noexcept { return { m_ui.mass_slider->value(), m_ui.radius_slider->value(), m_ui.orbit_slider->value() }; }

private:
	Ui::input_parameters_form m_ui;
};
