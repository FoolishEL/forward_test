#pragma once
#include "Rules.h"
#include "Car.h"

const int gaplen = 200;

//родительский класс для возможных будующих правил
class common_Rules 
{
public:
	//метод возвращает id нарушения или состояние разворота
	virtual int violation_id(car & CAR) = 0;
};

// правостороннее движение
class Rules : public common_Rules
{
private:
	//переменная максимального движение с одним направлением при повороте
	const int strike = 2;
	int strike_count;
	//индикатор поворота
	bool label_turn;
	int last_dir;
	//разрешённые места для разворота
	pair<int, int>*gaps;
	//количество разрешённых мест для разворота
	int gaps_am;
	//координаты середины дороги
	int xs;
	way * way_;
public:
	int violation_id(car & CAR) override;
	Rules(int gap,int xs,way * way);
	~Rules();
private:
	//пересечение границ дороги
	bool road_cross();

	//пересечение разделительной линии
	bool sep_cross();

	//движение против направления движения
	bool wrong_dir(car & CAR);

	//поворот
	int turn(car & CAR);
};

class Rules1 : public common_Rules 
{
private:
	const int strike = 2;
	int strike_count;
	bool label_turn;
	int last_dir;
	pair<int, int>*gaps;
	int gaps_am;
	int xs;
	way * way_;
public:
	int violation_id(car & CAR) override;
	Rules1(int gap, int xs, way * way);
	~Rules1();
private:
	bool road_cross();
	bool sep_cross();
	bool wrong_dir(car & CAR);
	int turn(car & CAR);
};
