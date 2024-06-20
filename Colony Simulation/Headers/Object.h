#pragma once
#include "Game.h"
class Dragon;

struct Messenger;

class Object
{
protected:
	std::string type;
	sf::RectangleShape model;
	sf::Texture* texture;
	sf::IntRect text_pos;
	bool was_update;
	bool is_alive, is_interactable;
public:
	Object();
	virtual ~Object() {}
	void Draw();
	std::string Get_Type();
	sf::Vector2f Get_Pos();
	void Reset_Update();
	sf::Vector2u Get_Index();
	bool Is_Alive();
	bool Is_Interactable();
	virtual void Update() = 0;
	virtual void Show() = 0;
	virtual int Interact_With_Dragon(Dragon * dragon, int damage) = 0;
};

