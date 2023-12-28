#include "Headers/Scaffolding.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include "Headers/House.h"
#include "Headers/Storage.h"
#include "Headers/Kitchen.h"
#include "Headers/Village.h"

Scaffolding::Scaffolding(sf::Vector2f pos, std::string type)
{
	building_time = 0;
	max_time = 10;
	this->type = "scaffolding";
	building_type = type;
	texture.loadFromFile("Images/Place_for_building.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(0, 0, vector.x / 2, vector.y));
}

Scaffolding::~Scaffolding()
{
	int y = Get_Index().y, x = Get_Index().x;
	if (Game::Get_Instance()->Get_Object(y, x)->Get_Type() == "scaffolding")
	{
		Game::Get_Instance()->Set_Object(y + 1, x, nullptr);
		Game::Get_Instance()->Set_Object(y, x + 1, nullptr);
		Game::Get_Instance()->Set_Object(y + 1, x + 1, nullptr);
	}
}

void Scaffolding::Interact()
{
	std::string building_task;
	if (building_type == "build house")
	{
		building_task = "house";
	}
	else if (building_type == "build storage")
		building_task = "storage";
	else
		building_task = "kitchen";
	if (Village::Get_Instance()->Can_Build(building_task))
	{
		was_update = true;
		building_time++;
		if (building_time >= max_time)
		{
			who_target->Reset_Target();
			Messenger* message = new Messenger;
			message->type = Types::death;
			message->death.dying = this;
			Game::Get_Instance()->Send_Message(message);
			Buildings* building;
			int y = Get_Index().y, x = Get_Index().x;
			if (building_type == "build house")
			{
				building = new House(Get_Pos());
			}
			else if (building_type == "build storage")
				building = new Storage(Get_Pos());
			else
				building = new Kitchen(Get_Pos());
			Game::Get_Instance()->Set_Object(y, x, building);
			Game::Get_Instance()->Set_Object(y + 1, x, building);
			Game::Get_Instance()->Set_Object(y, x + 1, building);
			Game::Get_Instance()->Set_Object(y + 1, x + 1, building);
			std::pair <int, int> cost = building->Get_Cost();
			Village::Get_Instance()->Change_Count(-cost.first, "tree");
			Village::Get_Instance()->Change_Count(-cost.second, "rock");
		}
	}
	else
	{
		who_target->Reset_Target();
		Messenger* message = new Messenger;
		message->type = Types::death;
		message->death.dying = this;
		Game::Get_Instance()->Send_Message(message);
	}
}

void Scaffolding::Update()
{
	if (who_target != nullptr)
	{
		int x = Get_Index().x, y = Get_Index().y;
		for (int i = y - 1; i <= y + 2; i++)
			for (int j = x - 1; j <= x + 2; j++)
			{
				if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
				{
					Object* object = Game::Get_Instance()->Get_Object(i, j);
					if (object != nullptr and object == who_target and not was_update)
						Interact();
				}
			}
	}
	else
	{
		who_target->Reset_Target();
		Messenger* message = new Messenger;
		message->type = Types::death;
		message->death.dying = this;
		Game::Get_Instance()->Send_Message(message);
	}
}
