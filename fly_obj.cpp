#include "fly_obj.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif /* !_USE_MATH_DEFINES */

#include <cmath>
#include <fstream>

auto fly_obj::fx(const double local_x) const noexcept
{
	double a{};
	for (auto&& [mass, x, y] : m_others)
	{
		auto r = std::hypot(x - local_x, y - m_y);

		a += mass * (x - local_x) / std::pow(r, 3);
	}

	return a;
}

auto fly_obj::fy(const double local_y) const noexcept
{
	double a{};
	for (auto&& [mass, x, y] : m_others)
	{
		auto r = std::hypot(x - m_x, y - local_y);

		a += mass * (y - local_y) / std::pow(r, 3);
	}

	return a;
}

void fly_obj::calc(double& coord, double& vel, fi_funct_t fi, const double precision) noexcept
{
	auto k1 = precision * (this->*fi)(coord);
	auto q1 = precision * vel;

	auto k2 = precision * (this->*fi)(coord + q1 / 2);
	auto q2 = precision * (vel + k1 / 2);

	auto k3 = precision * (this->*fi)(coord + q2 / 2);
	auto q3 = precision * (vel + k2 / 2);

	auto k4 = precision * (this->*fi)(coord + q3);
	auto q4 = precision * (vel + k3);

	coord += (q1 + 2 * q2 + 2 * q3 + q4) / 6;
	vel += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

fly_obj fly_obj::join(const fly_obj& lhs, const fly_obj& rhs)
{
	auto name = lhs.m_name + "+" + rhs.m_name;
	auto mass = lhs.m_mass + rhs.m_mass;

	auto x = (lhs.m_x + rhs.m_x) / 2;
	auto y = (lhs.m_y + rhs.m_y) / 2;

	auto vx = (lhs.m_vx * lhs.m_mass + rhs.m_vx * rhs.m_mass) / mass;
	auto vy = (lhs.m_vy * lhs.m_mass + rhs.m_vy * rhs.m_mass) / mass;

	auto obj = fly_obj(name, mass, x, y, vx, vy);
	
	auto color = lhs.m_mass > rhs.m_mass ? lhs.m_color : rhs.m_color;
	auto r = lhs.m_r * std::pow(mass / lhs.m_mass, 1. / 3.);
	obj.init_visual(color, r);

	return obj;
}

void fly_obj::init_visual(const QColor color, const double r)
{
	m_color = color;
	m_r = r;

	m_item = new QGraphicsEllipseItem(0, 0, 2 * m_r, 2 * m_r);
	m_item->setBrush(QBrush{ m_color });
}

void fly_obj::update(const double precision) noexcept
{
	calc(m_x, m_vx, &fly_obj::fx, precision);
	calc(m_y, m_vy, &fly_obj::fy, precision);

	m_others.clear();

	m_item->setPos({ m_x - m_r, m_y - m_r });
}

std::ifstream& operator>>(std::ifstream& ifstr, fly_obj& obj)
{
	int color;
	ifstr >> obj.m_name >> obj.m_x >> obj.m_y >> obj.m_vx >> obj.m_vy >> obj.m_mass >> obj.m_r >> color;

	obj.init_visual(Qt::GlobalColor(color), obj.m_r);
	return ifstr;
}