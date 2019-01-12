#include "Rules.h"
int Rules::violation_id(car & CAR)
{
	int answer = 0,tmp;
	if (this->road_cross())
		answer = 1;
	if (this->wrong_dir(CAR))
		answer = 3;
	if (this->sep_cross())
		answer = 2;
	if (tmp = this->turn(CAR))
		answer = tmp;
	return answer;
	
}

Rules::Rules(int gap, int xs,way * way)
{
	strike_count = 0;
	label_turn = false;  
	int sol;
	this->way_ = way;
	this->xs = xs;
	gaps_am = gap;
	if (gaps_am == 0) 
	{
		gaps = NULL;
	}
	else
	{

		gaps = new pair<int, int>[gap];
		sol = ((1000 - (gaps_am)*gaplen)) / (gaps_am + 1);
		for (int i = 0; i < gaps_am; i++)
		{
			gaps[i].first = (i+1)*sol+i*gaplen;
			gaps[i].second = gaps[i].first + gaplen;
		}
	}
}

Rules::~Rules()
{
	delete gaps;
}

bool Rules::road_cross()
{
	if (way_->get_at(way_->get_l()-1).first>775 || way_->get_at(way_->get_l()-1).first < 275)
		return true;
	return false;
}

bool Rules::sep_cross() 
{
	double tmp2 = 0;
	bool tmp = true;
	//начало и конец перемещения на разных сторонах дороги
	if (((way_->get_at(way_->get_l() - 1).first - xs)*
(way_->get_at(way_->get_l() - 2).first - xs)) <= 0)

		//пересечение сплошной линии
		if (gaps_am == 0)
			return true;
		else
		{
			//проверка каждого места для разворота
			for (int i = 0; i < gaps_am && tmp; i++) 
			{
				//перпендикулярное пересечение
				if(way_->get_at(way_->get_l()-1).second== way_->get_at(way_->get_l() - 2).second)
					if (way_->get_at(way_->get_l() - 1).second <= gaps[i].second&&way_->get_at(way_->get_l() - 1).second >= gaps[i].first)
						return false;

				//расчёт точки пересечения пути с центром дороги через уравнение прямой (деление на ноль исключено верхними условиями)
				tmp2 = ((525 - way_->get_at(way_->get_l() - 1).first) / (way_->get_at(way_->get_l() - 2).first - way_->get_at(way_->get_l() - 1).first))*
					(way_->get_at(way_->get_l() - 2).second - way_->get_at(way_->get_l() - 1).second) + way_->get_at(way_->get_l() - 1).second;
				//проверка что эта точка находится на разрешённом для разворота участке
				if (tmp2 <= gaps[i].second && tmp2 >= gaps[i].first)
					return false;
				
			}
			return true;
		}
	return false;
}

bool Rules::wrong_dir(car & CAR)
{
	if((CAR.get_dir()>=0)&&(CAR.get_dir()<=180)&&(CAR.get_x()>xs))
		return false;
	if (CAR.get_dir() >= 180 && CAR.get_dir() <= 360 && CAR.get_x() < xs)
		return false;
	return true;
}

int Rules::turn(car & CAR)
{
	if (label_turn == false)
		//если еще не в процессе разворота
	{
		if (!sep_cross())
			//если машина не пересекла разделительную полосу
		{
			if ((xs - way_->get_at(way_->get_l() - 1).first)*(xs - way_->get_at(way_->get_l() - 2).first) < 0)
				//но въехала на встречную полосу
			{
				label_turn = true;
				last_dir = CAR.get_dir();
				strike_count++;
				return 4;
			}
		}
	}
	else
		//в процессе разворота
	{
		if (wrong_dir(CAR)) 
			//и едет против шерсти
		{
			if (last_dir <= CAR.get_dir()) 
				//но выруливает
			{
				if (last_dir == CAR.get_dir()) 
					//при этом уже некоторое время сохраняет направление
				{
					if (strike_count == strike)
						//ограничение по продолжотильности сохранения направления
					{
						strike_count = 0;
						label_turn = false;
						last_dir = CAR.get_dir();
						return 3;
					}
					last_dir = CAR.get_dir();
					strike_count++;
					return 4;
				}
				//просто выруливает.
				last_dir = CAR.get_dir();
				strike_count = 0;
				return 4;

			}
			//а потом решила поехать против шерсти
			label_turn = false;
			strike_count = 0;
			last_dir = CAR.get_dir();
			return 3;
		}
		//и уже едет в направлении движении полосы
		//заничт разворот завершён
		if (CAR.get_dir() != 180 && CAR.get_dir() != 0 && CAR.get_dir() != 360)
			//разве что не перпендикулярно дороге
		{
			label_turn = false;
			strike_count = 0;
			return 5;
		}
		strike_count = 0;
		return 4;
	}
	last_dir = CAR.get_dir();
	return 0;
}

int Rules1::violation_id(car & CAR)
{
	int answer = 0, tmp;
	if (this->road_cross())
		answer = 1;
	if (this->wrong_dir(CAR))
		answer = 3;
	if (this->sep_cross())
		answer = 2;
	if (tmp = this->turn(CAR))
		answer = tmp;
	return answer;

}

Rules1::Rules1(int gap, int xs, way * way)
{
	strike_count = 0;
	label_turn = false;
	int sol;
	this->way_ = way;
	this->xs = xs;
	gaps_am = gap;
	if (gaps_am == 0)
	{
		gaps = NULL;
	}
	else
	{

		gaps = new pair<int, int>[gap];
		sol = ((1000 - (gaps_am)*gaplen)) / (gaps_am + 1);
		for (int i = 0; i < gaps_am; i++)
		{
			gaps[i].first = (i + 1)*sol + i*gaplen;
			gaps[i].second = gaps[i].first + gaplen;
		}
	}
}

Rules1::~Rules1()
{
	delete gaps;
}

bool Rules1::road_cross()
{
	if (way_->get_at(way_->get_l() - 1).first>775 || way_->get_at(way_->get_l() - 1).first < 275)
		return true;
	return false;
}

bool Rules1::sep_cross()
{
	double tmp2 = 0;
	bool tmp = true;
	if (((way_->get_at(way_->get_l() - 1).first - xs)*
		(way_->get_at(way_->get_l() - 2).first - xs)) <= 0)
		if (gaps_am == 0)
			return true;
		else
		{
			for (int i = 0; i < gaps_am && tmp; i++)
			{
				if (way_->get_at(way_->get_l() - 1).second == way_->get_at(way_->get_l() - 2).second)
					if (way_->get_at(way_->get_l() - 1).second <= gaps[i].second&&way_->get_at(way_->get_l() - 1).second >= gaps[i].first)
						return false;
				tmp2 = ((525 - way_->get_at(way_->get_l() - 1).first) / (way_->get_at(way_->get_l() - 2).first - way_->get_at(way_->get_l() - 1).first))*
					(way_->get_at(way_->get_l() - 2).second - way_->get_at(way_->get_l() - 1).second) + way_->get_at(way_->get_l() - 1).second;
				if (tmp2 <= gaps[i].second && tmp2 >= gaps[i].first)
					return false;

			}
			return true;
		}
	return false;
}

bool Rules1::wrong_dir(car & CAR)
{
	if (CAR.get_dir() >= 0 && CAR.get_dir() <= 180 && CAR.get_x()<xs)
		return false;
	if (CAR.get_dir() >= 180 && CAR.get_dir() <= 360 && CAR.get_x() > xs)
		return false;
	return true;
}

int Rules1::turn(car & CAR)
{
	if (label_turn == false)
		//если еще не в процессе разворота
	{
		if (!sep_cross())
			//если машина не пересекла разделительную полосу
		{
			if ((xs - way_->get_at(way_->get_l() - 1).first)*(xs - way_->get_at(way_->get_l() - 2).first) < 0)
				//но въехала на встречную полосу
			{
				label_turn = true;
				last_dir = CAR.get_dir();
				strike_count++;
				return 4;
			}
		}
	}
	else
		//в процессе разворота
	{
		if (wrong_dir(CAR))
			//и едет против шерсти
		{
			if (last_dir >= CAR.get_dir())
				//но выруливает
			{
				if (last_dir == CAR.get_dir())
					//при этом уже некоторое время сохраняет направление
				{
					if (strike_count == strike)
						//ограничение по продолжотильности сохранения направления
					{
						strike_count = 0;
						label_turn = false;
						last_dir = CAR.get_dir();
						return 3;
					}
					last_dir = CAR.get_dir();
					strike_count++;
					return 4;
				}
				//просто выруливает.
				last_dir = CAR.get_dir();
				strike_count = 0;
				return 4;

			}
			//а потом решила поехать против шерсти
			label_turn = false;
			strike_count = 0;
			last_dir = CAR.get_dir();
			return 3;
		}
		//и уже едет в направлении движении полосы
		//заничт разворот завершён
		if (CAR.get_dir() != 180 && CAR.get_dir() != 0 && CAR.get_dir() != 360)
			//разве что не перпендикулярно дороге
		{
			label_turn = false;
			strike_count = 0;
			return 5;
		}
		strike_count = 0;
		return 4;
	}
	last_dir = CAR.get_dir();
	return 0;
}
