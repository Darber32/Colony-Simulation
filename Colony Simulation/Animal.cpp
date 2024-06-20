#include "Headers/Animal.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"

void Animal::Death()
{
	is_alive = false;
	if (who_target != nullptr)
	{
		Village::Get_Instance()->Change_Count(meat_count, "raw meat");
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* slot = who_target->Get_Inventory(i);
			if (slot != nullptr)
			{
				if (slot->Get_Type() == "raw meat")
					meat_count = slot->Change_Count(meat_count);
				if (meat_count == 0)
					break;
			}
		}
		if (meat_count > 0)
			for (int i = 0; i < INVENTORY_SIZE; i++)
			{
				Inventory_Object* slot = who_target->Get_Inventory(i);
				if (slot == nullptr)
				{
					slot = new Inventory_Object("raw meat", meat_count);
					who_target->Set_Inventory(i, slot);
					break;
				}
			}
		if (who_target->Get_Target() == this)
			who_target->Reset_Target();
	}
	Messenger* message = new Messenger;
	message->type = Types::death;
	message->death.dying = this;
	Game::Get_Instance()->Send_Message(message);
}

Animal::Animal()
{
	is_interactable = true;
	is_alive = true;
}

void Animal::Take_Damage(int damage)
{
	hp -= damage;
}