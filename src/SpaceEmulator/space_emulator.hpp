#pragma once

#include <QtWidgets/QMainWindow>
#include <QEvent>
#include <QKeyEvent>

#include <tuple>
#include <filesystem>

#include "ui_space_emulator.h"
#include "fly_obj.hpp"

constexpr auto STARS_NUM = 200;

constexpr auto BORDER_SIZE = 5;
constexpr auto LOG_WIDTH = 220;
constexpr auto LOG_HEIGHT = 360;
constexpr auto LABEL_HEIGHT = 30;
constexpr auto SLIDER_HEIGHT = 30;

const std::filesystem::path CONFIG_DIR{ "../../cfg/" };

class space_emulator : public QMainWindow
{
	Q_OBJECT

private slots:
	void update();

private:
	// [[noreturn]]
	void show_error_and_exit(const std::string_view msg) /* const */;
	void show_warning(const std::string_view msg) /* const */;

	void make_bckg(const size_t stars_num = STARS_NUM);
	void load_config(const std::filesystem::path& path);
	std::tuple<double, double, double> get_input_parameters();

public:
	space_emulator(const bool fullscreen);
	~space_emulator() noexcept;

	void showEvent(QShowEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	Ui::solar_system m_ui;
	std::vector<fly_obj> m_objects;
	
	QGraphicsScene* m_scene;
	QTimer* m_timer;
	QImage* m_bckg;

	std::string m_status;

	size_t m_steps;
	double m_precision;
};
