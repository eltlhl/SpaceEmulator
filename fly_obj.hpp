#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <iostream>

#include <qgraphicsitem.h>
#include <qbrush.h>

constexpr auto OBJECTS_NUM = 9;

struct params
{
	double mass;
	double x;
	double y;
};

class fly_obj
{
	typedef double (fly_obj::*fi_funct_t)(const double) const;

private:
	auto fx(const double local_x) const noexcept;

	auto fy(const double local_y) const noexcept;

	void calc(double& coord, double& vel, fi_funct_t fi, const double precision) noexcept;

public:
	static fly_obj join(const fly_obj& lhs, const fly_obj& rhs);

public:
#pragma warning(suppress : 26495)
	inline fly_obj() noexcept = default;
	
	inline fly_obj(
		const std::string_view name,
		const double mass,
		const double x,
		const double y,
		const double vx,
		const double vy) noexcept :
		m_mass{ mass },
		m_x{ x },
		m_y{ y },
		m_vx{ vx },
		m_vy{ vy },
		m_r{ },
		m_name{ name },
		m_others{ },
		m_color{ Qt::white },
		m_item{ }
	{ }

	~fly_obj() noexcept
	{
		// delete m_item;
	}

	void init_visual(const QColor color, const double r);

	[[nodiscard]]
	__forceinline auto dist(const fly_obj& other) const noexcept { return std::hypot(m_x - other.m_x, m_y - other.m_y); }

	__forceinline void accel2(const fly_obj& other) { m_others.push_back({ other.m_mass, other.m_x, other.m_y }); }

	void update(const double precision) noexcept;

	inline void update_orbit(const double offset_x, const double offset_y) noexcept { m_x = offset_x - m_x; m_y = offset_y - m_y; }

	__forceinline void set_mass(const double mass) noexcept { m_mass = mass; }

	__forceinline void set_r(const double r) noexcept { m_r = r; m_item->setRect(0, 0, 2 * m_r, 2 * m_r); }

	__forceinline void set_color(QColor color) noexcept { m_color = color; m_item->setBrush(m_color); }

	[[nodiscard]]
	std::string_view get_name() const noexcept { return m_name; }

	[[nodiscard]]
	__forceinline auto get_x() const noexcept { return m_x; }

	[[nodiscard]]
	__forceinline auto get_y() const noexcept { return m_y; }

	[[nodiscard]]
	__forceinline auto get_r() const noexcept { return m_r; }

	[[nodiscard]]
	__forceinline auto get_gitem() noexcept { return m_item; }

	friend std::ifstream& operator>>(std::ifstream& ifstr, fly_obj& obj);

private:
	double m_mass;
	double m_x;
	double m_y;
	double m_vx;
	double m_vy;
	double m_r;
	std::string m_name;
	std::vector<params> m_others;

	QColor m_color;
	QGraphicsEllipseItem* m_item;
};