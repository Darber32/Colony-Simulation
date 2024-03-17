#pragma once
#define STORAGE_SIZE 10
#include "Buildings.h"
#include "Inventory_Object.h"

class Storage: public Buildings
{
private:
	Inventory_Object* storage[STORAGE_SIZE];
	int cur_time, max_time;
	bool is_full;
	void Interact();
public:
	Storage(sf::Vector2f pos);
	~Storage();
	Inventory_Object* Get_Storage(int index);
	void Reset_Storage(int index);
	void Show();
	bool Is_Full();
};

