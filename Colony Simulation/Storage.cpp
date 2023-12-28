#include "Headers/Storage.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"

Storage::Storage(sf::Vector2f pos)
{
	Village::Get_Instance()->Change_Count(1, "storage");
	type = "storage";
	cur_time = 0;
	max_time = 5;
	cost = std::pair<int, int>(20, 40);
	texture.loadFromFile("Images/Buildings/Wood/Barracks.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(vector.x / 4 * (rand() % 4), vector.y / 5 * (rand() % 2), vector.x / 4, vector.y / 5));
	for (int i = 0; i < STORAGE_SIZE; i++)
		storage[i] = nullptr;
}

Storage::~Storage()
{
	for (int i = 0; i < STORAGE_SIZE; i++)
		if (storage[i] != nullptr)
		{
			Village::Get_Instance()->Change_Count(-storage[i]->Get_Count(), storage[i]->Get_Type());
			delete storage[i];
		}
}

Inventory_Object* Storage::Get_Storage(int index)
{
	if (index >= STORAGE_SIZE)
		return nullptr;
	return storage[index];
}

void Storage::Interact()
{
	if (not is_anybody_inside)
	{
		Enter();
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* target_slot = who_target->Get_Inventory(i);
			if (target_slot != nullptr)
			{
				for (int j = 0; j < STORAGE_SIZE; j++)
				{
					if (storage[j] != nullptr and target_slot->Get_Type() == storage[j]->Get_Type())
					{
						int storage_count = storage[j]->Get_Count(), people_count = target_slot->Get_Count(), storage_avalible = storage[j]->Get_Max_Count() - storage_count;
						if (people_count <= storage_avalible)
						{
							storage[j]->Change_Count(people_count);
							target_slot->Change_Count(-people_count);
							Messenger* message = new Messenger;
							message->type = Types::empty_slot;
							message->empty_slot.index = i;
							message->empty_slot.people = who_target;
							message->empty_slot.slot = target_slot;
							Game::Get_Instance()->Send_Message(message);
						}
						else
						{
							storage[j]->Change_Count(storage_avalible);
							target_slot->Change_Count(-storage_avalible);
						}
					}
				}
			}
		}
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* target_slot = who_target->Get_Inventory(i);
			if (target_slot != nullptr and target_slot->Get_Count() != 0)
			{
				for (int j = 0; j < STORAGE_SIZE; j++)
					if (storage[j] == nullptr)
					{
						storage[j] = target_slot;
						who_target->Set_Inventory(i, nullptr);
						break;
					}
			}
		}
	}
	was_update = true;
	cur_time++;
	if (cur_time >= max_time)
	{
		cur_time = 0;
		Exit();
	}
}

