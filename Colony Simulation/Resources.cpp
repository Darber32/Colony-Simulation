#include "Headers/Resources.h"
#include "Headers/Messenger.h"
#include "Headers/Main.h"
#include "Headers/Village.h"

Resources::Resources(sf::Vector2f pos, std::string type)
{
	std::string filename;
	if (type == "tree")
		filename = "Images/Resources/Trees.png";
	else if (type == "rock")
		filename = "Images/Resources/Rocks.png";
	this->type = type;
	count_of_resources = rand() % 41 + 10;
	texture.loadFromFile(filename);
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(&texture);
	if (type == "tree")
		model.setTextureRect(sf::IntRect(vector.x / 4 * (rand() % 3 + 1) , 0, vector.x / 4, vector.y));
	else if (type == "rock")
	{
		model.setTextureRect(sf::IntRect(vector.x / 3 * (rand() % 3), vector.y / 4 * (rand() % 4), vector.x / 3, vector.y / 4));
	}
	interact_count = 0;
	interact_max = 5;
}

Resources::~Resources()
{
}

void Resources::Interact()
{
	who_target->Was_Update();
	interact_count++;
	if (interact_count == interact_max)
	{
		Village::Get_Instance()->Change_Count(count_of_resources, type);
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* slot = who_target->Get_Inventory(i);
			if (slot != nullptr)
			{
				if (slot->Get_Type() == type)
					count_of_resources = slot->Change_Count(count_of_resources);
				if (count_of_resources == 0)
					break;
			}
		}
		if (count_of_resources > 0)
			for (int i = 0; i < INVENTORY_SIZE; i++)
			{
				Inventory_Object* slot = who_target->Get_Inventory(i);
				if (slot == nullptr)
				{
					slot = new Inventory_Object(type, count_of_resources);
					who_target->Set_Inventory(i, slot);
					break;
				}
			}
		who_target->Reset_Target();
		Messenger* message = new Messenger;
		message->type = Types::death;
		message->death.dying = this;
		Game::Get_Instance()->Send_Message(message);
	}
}

void Resources::Update()
{
	if (who_target != nullptr)
	{
		int x = Get_Index().x, y = Get_Index().y;
		for (int i = y - 1; i <= y + 1; i++)
			for (int j = x - 1; j <= x + 1; j++)
			{
				if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
				{
					Object* object = Game::Get_Instance()->Get_Object(i, j);
					if (object != nullptr and object == who_target)
						Interact();
				}
			}
	}
}

int Resources::Get_Resources()
{
	return count_of_resources;
}
