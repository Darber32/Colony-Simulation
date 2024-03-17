#pragma once
#include "Buildings.h"
class Kitchen: public Buildings
{
private:
	void Interact();
public:
	Kitchen(sf::Vector2f pos);
	void Show();
};

