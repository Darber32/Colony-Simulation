#pragma once
#include "Buildings.h"
class Kitchen: public Buildings
{
private:
	void Interact();
public:
	Kitchen(sf::Vector2f pos, sf::Texture* tex);
	void Show();
};

