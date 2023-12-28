#include "Headers\Inventory_Object.h"

Inventory_Object::Inventory_Object(std::string type, int count)
{
	cur_count = count;
	this->type = type;
	if (type == "tree" or type == "rock")
		max_count = 50;
	else if (type == "raw meat" or type == "cooked meat")
		max_count = 100;
	if (cur_count > max_count)
		cur_count = max_count;
}

int Inventory_Object::Change_Count(int d) // возвращает остаток от добавления. Если -1, то невозможно использовать
{
	if (cur_count + d >= 0)
	{
		int temp;
		cur_count += d;
		if (cur_count > max_count)
		{
			temp = cur_count - max_count;
			cur_count = max_count;
			return temp;
		}
		return 0;
	}
	return -1;
}

std::string Inventory_Object::Get_Type()
{
	return type;
}

int Inventory_Object::Get_Count()
{
	return cur_count;
}

int Inventory_Object::Get_Max_Count()
{
	return max_count;
}
