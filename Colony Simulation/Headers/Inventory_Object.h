#pragma once
#include "Game.h"

class Inventory_Object
{
private:
	std::string type;
	int cur_count, max_count;
public:
	Inventory_Object(std::string type, int count);
	int Change_Count(int d);
	std::string Get_Type();
	int Get_Count();
	int Get_Max_Count();
};

