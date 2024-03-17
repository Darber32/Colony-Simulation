#pragma once
#include "Interactable_Object.h"
class Resources : public Interactable_Object
{
private:
	int count_of_resources, interact_count, interact_max;
	void Interact();
public:
	Resources(sf::Vector2f pos, std::string type);
	~Resources();
	void Update();
	int Get_Resources();
	void Show();
};

