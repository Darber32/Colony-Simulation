#include "Headers/Kitchen.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"

Kitchen::Kitchen(sf::Vector2f pos)
{
	Village::Get_Instance()->Change_Count(1, "kitchen");
	type = "kitchen";
	cost = std::pair<int, int>(40, 20);
	texture.loadFromFile("Images/Buildings/Wood/Market.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(vector.x / 3 * (rand() % 3), vector.y / 4 * (rand() % 4), vector.x / 3, vector.y / 4));
}

void Kitchen::Interact()
{
	if (not is_anybody_inside)
	{
		Enter();
	}
	was_update = true;
	Inventory_Object* meat = nullptr;
	int index;
	bool flag = true;
	int c = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (who_target->Get_Inventory(i) != nullptr)
		{ 
			c++;
			if (who_target->Get_Inventory(i)->Get_Type() == "raw meat")
			{
				meat = who_target->Get_Inventory(i);
				index = i;
			}
		}
	}
	if (c == INVENTORY_SIZE or meat == nullptr or meat->Get_Count() == 0)
	{
		flag = false;
		Exit();
	}
	if (flag and meat != nullptr)
	{
		int count = meat->Get_Count();
		if (count > 5)
			count = 5;
		for (int i = 0; i < INVENTORY_SIZE; i++)
			if (who_target->Get_Inventory(i) and who_target->Get_Inventory(i)->Get_Type() == "cooked meat")
			{
				Village::Get_Instance()->Change_Count(-count, "raw meat");
				meat->Change_Count(-count);
				Village::Get_Instance()->Change_Count(count, "cooked meat");
				count = who_target->Get_Inventory(i)->Change_Count(count);
				meat->Change_Count(count);
				if (count == 0)
					break;
			}
		if (count > 0)
			for (int i = 0; i < INVENTORY_SIZE; i++)
				if (who_target->Get_Inventory(i) == nullptr)
				{
					Inventory_Object* cooked_meat = new Inventory_Object("cooked meat", count);
					Village::Get_Instance()->Change_Count(-count, "raw meat");
					meat->Change_Count(-count);
					Village::Get_Instance()->Change_Count(count, "cooked meat");
					who_target->Set_Inventory(i, cooked_meat);
					break;
				}
	}
	if (meat != nullptr and meat->Get_Count() == 0)
	{
		Messenger* message = new Messenger;
		message->type = Types::empty_slot;
		message->empty_slot.index = index;
		message->empty_slot.people = who_target;
		message->empty_slot.slot = meat;
		Game::Get_Instance()->Send_Message(message);
	}
}
