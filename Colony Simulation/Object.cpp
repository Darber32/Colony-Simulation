#include "Headers/Object.h"
#include "Headers/Game.h"
#include "Headers/Main.h"

Object::Object()
{
	was_update = false;
}

void Object::Draw()
{
	window.draw(model);
}

std::string Object::Get_Type()
{
	return type;
}

sf::Vector2f Object::Get_Pos()
{
	return model.getPosition();
}

void Object::Reset_Update()
{
	if (was_update)
		was_update = false;
}

sf::Vector2u Object::Get_Index()
{
	return sf::Vector2u(model.getPosition().x / (WIN_WIDTH / MAP_WIDTH), model.getPosition().y / (WIN_HEIGHT / MAP_HEIGHT));
}
