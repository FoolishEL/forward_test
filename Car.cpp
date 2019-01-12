#pragma once
#include "Car.h"

car::car()
{
	x = y = dir = 0;
}

car::car(double x, double y, int dir) 
{
	this->x = x;
	this->y = y;
	this->dir = dir;
}

void car::left() 
{
	dir += d_left;
	if (dir > 360)
		dir -= 360;
}

void car::right()
{
	dir -= d_left;
	if (dir < 0)
		dir += 360;
}

bool car::forward(bool(*road)(double nx, double ny), way & in)
{
	bool tmp;
	double dx, dy;
	dx = speed*cos(dir*pi / 180);
	dy = speed*sin(dir*pi / 180);
	tmp = road(x + dx, y + dy);
	if (tmp) 
	{
		x += dx;
		y += dy;
		in.add_point(x, y);
	}
	return tmp;
}
double const car::get_x()
{
	return x;
}
double const car::get_y()
{
	return y;
}

void car::set_x(double xn)
{
	x = xn;
}

void car::set_y(double yn)
{
	y = yn;
}

int const car::get_dir()
{
	return dir;
}

double const car::get_s()
{
	return speed;
}

void way::add_point(double x, double y)
{
	pair<double, double> tmp = make_pair(x, y);
	deq.push_back(tmp);
	if (deq.size() > 16)
		deq.pop_front();
}
pair<double, double> way::get_at(int i)
{
	if (i >= 0 && i < (deq.size())) {
		return deq.at(i);
	}
	return pair<double, double>(0, 0);
}
int way::get_l() 
{
	return deq.size();
}

way::~way()
{
	deq.clear();
}
