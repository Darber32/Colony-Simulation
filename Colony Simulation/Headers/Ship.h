#pragma once
#include "Object.h"

class Ship: public Object
{
public:
	Ship(sf::Vector2f pos, sf::Texture* tex);
	~Ship();
	void Update() {}
	void Show();
	int Interact_With_Dragon(Dragon* dragon, int damage) { return 0; }
};

