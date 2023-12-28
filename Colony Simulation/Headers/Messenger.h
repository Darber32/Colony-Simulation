#pragma once
#include "People.h"
#include "Animal.h"

enum class Types
{
	death, empty_slot, move, attack_animal, attack_people
};

struct Messenger
{
	Messenger() {}
	Types type;
	union
	{
		struct
		{
			Object* dying;
		} death;
		struct
		{
			People* people;
			Inventory_Object* slot;
			int index;
		} empty_slot;
		struct
		{
			People* target;
			Animal * animal;
			sf::Vector2u pos;
		} move;
		struct
		{
			Animal* target;
			int damage;
		} attack_animal;
		struct
		{
			People* target;
			int damage;
		} attack_people;
	};
};

