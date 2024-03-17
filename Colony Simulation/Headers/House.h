#pragma once
#include "Buildings.h"

class House : public Buildings
{
private:
	int sleep_count, max_sleep;
	void Interact();
public:
	House(sf::Vector2f pos);
	void Show();
};

