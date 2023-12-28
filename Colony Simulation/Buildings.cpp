#include "Headers/Buildings.h"

Buildings::Buildings()
{
	old_position = sf::Vector2u(0, 0);
	is_anybody_inside = false;
}

Buildings::~Buildings()
{
	int y = Get_Index().y, x = Get_Index().x;
	Game::Get_Instance()->Set_Object(y + 1, x, nullptr);
	Game::Get_Instance()->Set_Object(y, x + 1, nullptr);
	Game::Get_Instance()->Set_Object(y + 1, x + 1, nullptr);
}
void Buildings::Enter()
{
	old_position = who_target->Get_Index();
	who_target->Enter_In_Building();
	is_anybody_inside = true;
}

void Buildings::Exit()
{
	if (Game::Get_Instance()->Get_Object(old_position.y, old_position.x) == nullptr)
	{
	who_target->Exit_The_Building(old_position);
	who_target->Reset_Target();
	who_target = nullptr;
	is_anybody_inside = false;	
	}
	else
	{
		int x = Get_Index().x, y = Get_Index().y;
		for (int i = y - 1; i <= y + 2; i++)
			for (int j = x - 1; j <= x + 2; j++)
				if (Game::Get_Instance()->Get_Object(i, j) == nullptr)
				{
					old_position.x = j;
					old_position.y = i;
					i = y + 3;
					break;
				}
		who_target->Exit_The_Building(old_position);
		who_target->Reset_Target();
		who_target = nullptr;
		is_anybody_inside = false;
	}
}


bool Buildings::Is_Anybody_Inside()
{
	return is_anybody_inside;
}

void Buildings::Update()
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
	else if (is_anybody_inside)
		is_anybody_inside = false;
}

std::pair<int, int> Buildings::Get_Cost()
{
	return cost;
}
