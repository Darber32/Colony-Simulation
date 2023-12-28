#pragma once
#include "Interactable_Object.h"

class Buildings: public Interactable_Object
{
protected:
	std::pair<int, int> cost;
	sf::Vector2u old_position;
	bool is_anybody_inside;
	void Enter();
	void Exit();
public:
	Buildings();
	~Buildings();
	bool Is_Anybody_Inside();
	void Update();
	std::pair <int, int> Get_Cost();
};

