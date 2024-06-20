#pragma once
#include "Interactable_Object.h"

class Scaffolding : public Interactable_Object
{
private:
	std::string building_type;
	int building_time, max_time;
public:
	Scaffolding(sf::Vector2f pos, std::string type, sf::Texture* tex);
	~Scaffolding();
	void Interact();
	void Update();
	void Show();
	int Interact_With_Dragon(Dragon* dragon, int damage) { return 0; }
};

