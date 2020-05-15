#include "space_emulator.hpp"
#include "input_parameters_form.hpp"

#include <QTimer.h>
#include <QGraphicsScene.h>
#include <QPainter.h>
#include <QGraphicsItem.h>
#include <QMessageBox.h>

#include <cmath>
#include <fstream>

void space_emulator::update()
{
	static constexpr auto MAX_DIST = 1000;

	for (size_t i{}; i < m_steps; ++i)
	{
		std::string_view collapsed1,
			 collapsed2,
		 out_of;
		for (auto&& obj1 : m_objects)
		{
	for (auto&& obj2 : m_objects)
	if (obj1.get_name() 
	 != obj2.get_name())
		{
	if (auto dist 
	 = obj1.dist(obj2);
	dist <= obj1.get_r() + obj2.get_r())
					{
	collapsed1 = obj1.get_name();
	ollapsed2 = obj2.get_name();
					}
			else
	obj1.accel2(obj2);
				}
if (obj1.get_name() != "Sun" && 
    obj1.dist(m_objects[0]) > MAX_DIST)
	out_of = obj1.get_name();
		}

	if (!collapsed1.empty())
		{
	m_status += std::string("COLLAPSED (") + collapsed1.data() 
		+ "+" + collapsed2.data() + ")\n";
			m_ui.log->setText(m_status.data());

size_t i = -1, j = -1;
for (size_t k{}; k < m_objects.size(); ++k)
		if (i != -1 && j != -1)
					break;
else if (m_objects[k].get_name() == collapsed1)
					i = k;
	else if (m_objects[k].get_name() == collapsed2)
					j = k;

	m_scene->removeItem(m_objects[i].get_gitem());
	m_scene->removeItem(m_objects[j].get_gitem());
		m_objects[i] = fly_obj::join(m_objects[i], m_objects[j]);
		m_scene->addItem(m_objects[i].get_gitem()); 
			
	m_objects.erase(std::begin(m_objects) + j);
		}

		if (!out_of.empty())
		{
	m_status += std::string("OUT OF RANGE(") + out_of.data() + ")\n";
	m_ui.log->setText(m_status.data());

m_objects.erase(std::find_if(std::begin(m_objects), 
			     std::end(m_objects), [&](const auto& x) 
			     { return x.get_name() == out_of; }));
		}

		for (auto&& obj : m_objects)
			obj.update(m_precision);
	}
}

void space_emulator::show_error_and_exit(const std::string_view msg)
{
	QMessageBox _(QMessageBox::Icon::Critical,
		"Warning",
		msg.data(),
		QMessageBox::StandardButton::Ok,
		this);

	_.exec();

	close();
}

void space_emulator::show_warning(const std::string_view msg)
{
	QMessageBox _(QMessageBox::Icon::Warning,
		"Warning",
		msg.data(),
		QMessageBox::StandardButton::Ok,
		this);

	_.exec();
}

void space_emulator::make_bckg(const size_t stars_num)
{
	QPainter painter(m_bckg);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::black);
	painter.drawRect(0, 0, m_ui.canvas->width(), m_ui.canvas->height());

	painter.setPen(Qt::white);
	for (size_t i{}; i < stars_num; ++i)
		painter.drawPoint(rand() % m_ui.canvas->width(), rand()
				  % m_ui.canvas->height());
}

void space_emulator::load_config(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		show_error_and_exit("Config file does not exist.\nExiting...");

		return;
	}


	std::ifstream input(path);
	if (!input.is_open())
	{
		show_error_and_exit("Could not open config file.\nExiting...");

		return;
	}
	
	std::string dummy;
	std::getline(input, dummy);

	double offset_x = m_ui.canvas->width() / 2.;
	double offset_y = m_ui.canvas->height() / 2.;
	for (auto&& obj : m_objects)
	{
		input >> obj;

		obj.update_orbit(offset_x, offset_y);
	}

	input.close();

	if (auto it = std::find_if(std::begin(m_objects), std::end(m_objects), 
				   
	[](const auto& x) { return x.get_name() == "Earth"; }); it != std::end(m_objects))
	{
		auto&& [mass, r, orbit] = get_input_parameters();

		it->set_mass(mass);
		it->set_r(r);
		it->set_orbit(offset_x - orbit);
	}
	else
		show_warning("Could not find an object named 
			     \"Earth\".\nLoading default values.");
}

std::tuple<double, double, double> space_emulator::get_input_parameters()
{
	input_parameters_form dialog(this);

	dialog.exec();

	return dialog.get_values();
}

space_emulator::space_emulator(const bool fullscreen) :
	QMainWindow{},
	m_objects(OBJECTS_NUM),
	m_scene{},
	m_timer{},
	m_bckg{},
	m_status{},
	m_steps{ 5 },
	m_precision{ 0.1 }
{
	m_ui.setupUi(this);
	
	if (fullscreen)
		setWindowState(Qt::WindowFullScreen);

	connect(m_ui.steps_slider, &QSlider::valueChanged, 
	[this]() noexcept { m_steps = m_ui.steps_slider->value(); });
	connect(m_ui.precision_slider, &QSlider::valueChanged,
[this]() noexcept { m_precision = 1. / m_ui.precision_slider->value(); });

	m_scene = new QGraphicsScene(m_ui.canvas);
	m_ui.canvas->setRenderHint(QPainter::Antialiasing);

	m_bckg = new QImage(m_ui.canvas->width(),
m_ui.canvas->height(), QImage::Format_RGB16);
	make_bckg();

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

	auto path = CONFIG_DIR;
	load_config(path += "solar_system.cfg");
}

space_emulator::~space_emulator() noexcept
{
	delete m_timer;
	delete m_bckg;
	delete m_scene;
}

void space_emulator::showEvent(QShowEvent* event)
{
#pragma warning(suppress : 26451)
	m_scene->setSceneRect(0, 0, m_ui.canvas->width() - 2, m_ui.canvas->height() - 2);
	m_ui.canvas->setScene(m_scene);
	
	m_scene->setBackgroundBrush(m_bckg->scaledToHeight(m_ui.canvas->height()));
	for (auto&& obj : m_objects)
		m_scene->addItem(obj.get_gitem());

	m_timer->start(20);

	QMainWindow::showEvent(event);
}

void space_emulator::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
		close();

	QMainWindow::keyPressEvent(event);
}

void space_emulator::resizeEvent(QResizeEvent* event)
{
	auto canvas_width = width() - LOG_WIDTH - 3 * BORDER_SIZE;
	auto h = height() - 2 * BORDER_SIZE;
	
	m_ui.canvas->resize({ canvas_width, h });
	
	auto fixed_widgets_x = canvas_width + 2 * BORDER_SIZE;
	auto y_it{ 0 };
	m_ui.log->move({ fixed_widgets_x, y_it += BORDER_SIZE });

	m_ui.steps_label->move({ fixed_widgets_x, y_it += LOG_HEIGHT + BORDER_SIZE });
	m_ui.steps_slider->move({ fixed_widgets_x, y_it += LABEL_HEIGHT + BORDER_SIZE });

	m_ui.precision_label->move({ fixed_widgets_x, y_it += SLIDER_HEIGHT + BORDER_SIZE });
	m_ui.precision_slider->move({ fixed_widgets_x, y_it += LABEL_HEIGHT + BORDER_SIZE });

	QMainWindow::resizeEvent(event);
}
