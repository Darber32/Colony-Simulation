#pragma once
#include "People.h"
#include "Animal.h"
#include "Enemy.h"
#include "Thief.h"

enum class Types
{
	death, empty_slot, attack_animal, attack_people, attack_enemy, attack_thief
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
		/*struct
		{
			People* people;
			Inventory_Object* slot;
			int index;
		} empty_slot;*/
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
		struct
		{
			Enemy* target;
			int damage;
		} attack_enemy;
		struct
		{
			Thief* target;
			int damage;
		} attack_thief;
	};
};

