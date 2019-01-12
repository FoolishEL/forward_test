#pragma once
#include "Car.h"
#include <math.h>
#include <deque>

using namespace std;
const double pi = 3.1415926535;
class way;

class car
{
public:
	car();
	car(double x, double y, int dir);
	void left();
	void right();
	bool forward(bool(*road)(double nx, double ny),way &in);
	double const get_x();
	double const get_y();
	void set_x(double xn);
	void set_y(double yn);
	int const get_dir();
	double const get_s();

private:
	double x, y;
	int dir;
	const int d_left = 10;
	const int speed = 30;
};

class way 
{
private:
	deque<pair<double,double>> deq;
public:
	void add_point(double x,double y);
	pair<double, double> get_at(int i);
	int get_l();
	~way();
};
