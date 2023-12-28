#pragma once
#include "Game.h"

struct Messenger;

class Object
{
protected:
	std::string type;
	sf::RectangleShape model;
	sf::Texture texture;
	bool was_update;
public:
	Object();
	virtual ~Object() {}
	void Draw();
	std::string Get_Type();
	sf::Vector2f Get_Pos();
	void Reset_Update();
	sf::Vector2u Get_Index();
	virtual void Update() = 0;
};

