#include "Headers/People.h"
#include "Headers/Main.h"
#include "Headers/Game.h"
#include "Headers/Interactable_Object.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"
#include "Headers/Buildings.h"
#include "Headers/Storage.h"
#include "Headers/Scaffolding.h"

People::People(sf::Vector2f pos, std::string type)
{
	std::string filename;
	target = nullptr;
	this->type = "people";
	task_type = type;
	if (task_type == "woodcutter")
	{
		priority["woodcutter"] = 4;
		priority["miner"] = rand() % 3 + 1;
		priority["hunter"] = rand() % 3 + 1;
		priority["builder"] = 0;
		filename = "Images/People/Green_people.png";
	}
	else if (task_type == "miner")
	{
		priority["woodcutter"] = rand() % 3 + 1;
		priority["miner"] = 4;
		priority["hunter"] = rand() % 3 + 1;
		priority["builder"] = 0;
		filename = "Images/People/Blue_people.png";
	}
	else if (task_type == "hunter")
	{
		priority["woodcutter"] = rand() % 3 + 1;
		priority["miner"] = rand() % 3 + 1;
		priority["hunter"] = 4;
		priority["builder"] = 0;
		filename = "Images/People/Red_people.png";
	}
	else if (task_type == "builder")
	{
		priority["woodcutter"] = rand() % 3 + 1;
		priority["miner"] = rand() % 3 + 1;
		priority["hunter"] = 0;
		priority["builder"] = 4;
		filename = "Images/People/Purple_people.png";
	}
	heal_points = 20;
	max_heal_point = 20;
	max_hunger_points = 50;
	hunger_points = max_hunger_points;
	damage = 1;
	max_sleep = 20;
	sleep_count = max_sleep;
	in_building = false;
	build_time = 0;
	build_max = 10;
	texture.loadFromFile(filename);
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(0, vector.y / 12, vector.x / 5, vector.y / 12));
	for (int i = 0; i < INVENTORY_SIZE; i++)
		inventory[i] = nullptr;
}

People::~People()
{
	if (target != nullptr)
		target->Set_Target_People(nullptr);
	Village::Get_Instance()->Change_Count(-1, "people");
	for (int i = 0; i < INVENTORY_SIZE; i++)
		if (inventory[i] != nullptr)
		{
			Village::Get_Instance()->Change_Count(-inventory[i]->Get_Count(), inventory[i]->Get_Type());
			delete inventory[i];
		}
}

void People::Move()
{
	sf::Vector2u pos = Get_Index();
	sf::Vector2u new_pos = way[way.size() - 1];
	if (Game::Get_Instance()->Get_Object(new_pos.y, new_pos.x) != nullptr)
	{
		Find_Shortest_Way();
		if (not way.empty())
			new_pos = way[way.size() - 1];
	}
	if (not way.empty())
	{
		way.pop_back();
		if (pos.y > new_pos.y)
			Change_Texture(0);
		else if (pos.y < new_pos.y)
			Change_Texture(1);
		else if (pos.x > new_pos.x)
			Change_Texture(2);
		else if (pos.x < new_pos.x)
			Change_Texture(3);
		Game::Get_Instance()->Set_Object(new_pos.y, new_pos.x, this);
		Game::Get_Instance()->Set_Object(pos.y, pos.x, nullptr);
		pos.x = new_pos.x * (WIN_WIDTH / MAP_WIDTH);
		pos.y = new_pos.y * (WIN_HEIGHT / MAP_HEIGHT);
		model.setPosition((sf::Vector2f)pos);
		Was_Update();
	}
}

void People::Update()
{
	if (not was_update)
	{
		if (heal_points > 0)
		{
			if (hunger_points <= 20 or heal_points <= 2)
				Eat();
			if (target == nullptr)
			{
				if (heal_points <= 7)
					Find_Nearest_Object("house");
				if (target == nullptr)
				{
					int c = 0;
					for (int i = 0; i < INVENTORY_SIZE; i++)
						if (inventory[i] != nullptr)
							c++;
					if (c > 0)
						Find_Nearest_Object("storage");
					if (target == nullptr)
					{
						std::string task = Village::Get_Instance()->Find_Task(this);
						if (task == "build house" or task == "build kitchen" or task == "build storage")
							Find_Place_For_Building(task);
						else
							Find_Nearest_Object(task);
					}
				}
			}
			if (target != nullptr and way.empty())
			{
				target_pos = (sf::Vector2u)target->Get_Pos();
				Find_Shortest_Way();
			}
			if (not way.empty() and not in_building)
				Move();
			if (not in_building)
			{
				sleep_count--;
				if (sleep_count <= 0)
				{
					heal_points--;
					sleep_count += max_sleep / 2;
				}
				hunger_points--;
				if (hunger_points <= 0)
				{
					heal_points--;
					hunger_points = max_hunger_points / 2;
				}
			}
		}
		else
		{
			Messenger* message = new Messenger;
			message->type = Types::death;
			message->death.dying = this;
			Game::Get_Instance()->Send_Message(message);
		}
	}
}

void People::Find_Nearest_Object(std::string type)
{
	int my_i = Get_Index().y, my_j = Get_Index().x;
	std::vector<Interactable_Object*> arr;
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			Interactable_Object* object = (Interactable_Object*)Game::Get_Instance()->Get_Object(i, j);
			if (object != nullptr)
			{
				if (object->Get_Type() == "house" or object->Get_Type() == "storage" or object->Get_Type() == "kitchen")
				{
					Buildings* building = (Buildings*)object;
					if (building->Get_Type() == type and not building->Is_Anybody_Inside() and object->Get_Target_People() == nullptr)
						arr.push_back(object);
				}
				else
					if (object->Get_Type() == type and object->Get_Target_People() == nullptr)
						arr.push_back(object);
			}
		}
	int min = INT_MAX, cur = 0;
	for (int i = 0; i < arr.size(); i++)
	{
		int x = abs(my_j - (int)arr[i]->Get_Index().x);
		int y = abs(my_i - (int)arr[i]->Get_Index().y);
		cur = x + y;
		if (cur < min)
		{
			if (target != nullptr and target->Get_Target_People() != nullptr)
				target->Set_Target_People(nullptr);
			min = cur;
			target = arr[i];
			target->Set_Target_People(this);
			target_pos = (sf::Vector2u)target->Get_Pos();
		}
	}
}

void People::Find_Place_For_Building(std::string type)
{
	for (int i = 3; i < MAP_HEIGHT - 5; i++)
		for (int j = 3; j < MAP_WIDTH - 5; j++)
		{
			if (Chech_Place(i, j))
			{
				target = new Scaffolding(sf::Vector2f(j * (WIN_WIDTH / MAP_WIDTH), i * (WIN_HEIGHT / MAP_HEIGHT)), type);
				Game::Get_Instance()->Set_Object(i, j, target);
				Game::Get_Instance()->Set_Object(i + 1, j, target);
				Game::Get_Instance()->Set_Object(i, j + 1, target);
				Game::Get_Instance()->Set_Object(i + 1, j + 1, target);
				target->Set_Target_People(this);
				target_pos = (sf::Vector2u)target->Get_Pos();
				i = MAP_HEIGHT;
				break;
			}
		}
}

void People::Find_Shortest_Way()
{
	way.clear();
	bool flag = true;
	int map[MAP_HEIGHT][MAP_WIDTH];
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			map[i][j] = 99;
	std::vector<sf::Vector2u> wave, old_wave;
	int x = Get_Index().x, y = Get_Index().y;
	old_wave.push_back(sf::Vector2u(x, y));
	int step = 0;
	map[y][x] = step;
	while (not old_wave.empty() and flag)
	{
		step++;
		wave.clear();
		for (std::vector<sf::Vector2u>::iterator i = old_wave.begin(); i != old_wave.end(); i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int new_x = i->x, new_y = i->y;
				switch (j)
				{
				case 0:
					if (new_x != MAP_WIDTH - 1)
						if (Game::Get_Instance()->Get_Object(new_y, new_x + 1) == nullptr and map[new_y][new_x + 1] > step)
							new_x++;
					break;
				case 1:
					if (new_x != 0)
						if (Game::Get_Instance()->Get_Object(new_y, new_x - 1) == nullptr and map[new_y][new_x - 1] > step)
							new_x--;
					break;
				case 2:
					if (new_y != MAP_HEIGHT - 1)
						if (Game::Get_Instance()->Get_Object(new_y + 1, new_x) == nullptr and map[new_y + 1][new_x] > step)
							new_y++;
					break;
				case 3:
					if (new_y != 0)
						if (Game::Get_Instance()->Get_Object(new_y - 1, new_x) == nullptr and map[new_y - 1][new_x] > step)
							new_y--;
					break;
				}
				if (flag and (new_x != i->x or new_y != i->y))
				{
					wave.push_back(sf::Vector2u(new_x, new_y));
					map[new_y][new_x] = step;
				}
			}
		}
		old_wave = wave;
	}
	wave.clear();
	old_wave.clear();
	/*for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
			if (map[i][j] > 10)
				std::cout << map[i][j] << " ";
			else
				std::cout << " " << map[i][j] << " ";
		std::cout << std::endl;
	}*/
	if (target != nullptr)
	{
		if (target->Get_Type() == "house" or target->Get_Type() == "storage" or target->Get_Type() == "kitchen" or target->Get_Type() == "scaffolding")
			Way_For_Building(map);
		else
			Way_For_Object(map);
	}
}

void People::Change_Texture(int type)
{
	sf::Vector2u vector = texture.getSize();
	model.setTextureRect(sf::IntRect(0, vector.y / 12 * type, vector.x / 5, vector.y / 12));
}

void People::Eat()
{
	int c = 5;
	for (int i = 0; i < INVENTORY_SIZE; i++)
		if (inventory[i] != nullptr and inventory[i]->Get_Type() == "cooked meat")
			if (inventory[i]->Get_Count() >= c)
			{
				inventory[i]->Change_Count(-c);
				Village::Get_Instance()->Change_Count(-c, "cooked meat");
				c = 0;
			}
			else
			{
				Village::Get_Instance()->Change_Count(-inventory[i]->Get_Count(), "cooked meat");
				c = inventory[i]->Change_Count(-inventory[i]->Get_Count());
			}
	if (c > 0)
	{
		for (int i = 0; i < MAP_HEIGHT; i++)
			for (int j = 0; j < MAP_WIDTH; j++)
				if (Game::Get_Instance()->Get_Object(i, j) != nullptr and Game::Get_Instance()->Get_Object(i, j)->Get_Type() == "storage" and c > 0)
				{
					Storage* storage = (Storage*)Game::Get_Instance()->Get_Object(i, j);
					for (int k = 0; k < STORAGE_SIZE; k++)
						if (storage->Get_Storage(k) != nullptr and storage->Get_Storage(k)->Get_Type() == "cooked meat" and storage->Get_Storage(k)->Get_Count() > 0)
							if (storage->Get_Storage(k)->Get_Count() >= c)
							{
								storage->Get_Storage(k)->Change_Count(-c);
								Village::Get_Instance()->Change_Count(-c, "cooked meat");
								c = 0;
							}
							else
							{
								Village::Get_Instance()->Change_Count(-storage->Get_Storage(k)->Get_Count(), "cooked meat");
								c = storage->Get_Storage(k)->Change_Count(-storage->Get_Storage(k)->Get_Count());
							}
				}
	}
	if (c <= 0)
	{
		heal_points += 5;
		hunger_points = max_hunger_points;
	}

}

bool People::Chech_Place(int i, int j)
{
	for (int k = i - 1; k <= i + 2; k++)
		for (int g = j - 1; g <= j + 2; g++)
			if (Game::Get_Instance()->Get_Object(k, g) != nullptr)
				return false;
	return true;
}

void People::Way_For_Building(int map[MAP_HEIGHT][MAP_WIDTH])
{
	int x = target->Get_Index().x, y = target->Get_Index().y;
	if (Get_Index().x > x)
		x++;
	if (Get_Index().y > y)
		y++;
	int min = INT_MAX, min_x, min_y;
	while (map[y][x] != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			int new_x = x, new_y = y;
			switch (i)
			{
			case 0:
				if (x != MAP_WIDTH - 1)
					new_x++;
				break;
			case 1:
				if (x != 0)
					new_x--;
				break;
			case 2:
				if (y != MAP_HEIGHT - 1)
					new_y++;
				break;
			case 3:
				if (y != 0)
					new_y--;
				break;
			}
			if (x != new_x or y != new_y)
				if (map[new_y][new_x] < min)
				{
					min = map[new_y][new_x];
					min_x = new_x;
					min_y = new_y;
				}
		}
		x = min_x;
		y = min_y;
		if (map[y][x] != 0)
			way.push_back(sf::Vector2u(x, y));
	}
}

void People::Way_For_Object(int map[MAP_HEIGHT][MAP_WIDTH])
{

	int x = target->Get_Index().x, y = target->Get_Index().y;
	int min = INT_MAX, min_x, min_y;
	while (map[y][x] != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			int new_x = x, new_y = y;
			switch (i)
			{
			case 0:
				if (x != MAP_WIDTH - 1)
					new_x++;
				break;
			case 1:
				if (x != 0)
					new_x--;
				break;
			case 2:
				if (y != MAP_HEIGHT - 1)
					new_y++;
				break;
			case 3:
				if (y != 0)
					new_y--;
				break;
			}
			if (x != new_x or y != new_y)
				if (map[new_y][new_x] < min)
				{
					min = map[new_y][new_x];
					min_x = new_x;
					min_y = new_y;
				}
		}
		x = min_x;
		y = min_y;
		if (map[y][x] != 0)
			way.push_back(sf::Vector2u(x, y));
	}
}

void People::Was_Update()
{
	if (not was_update)
		was_update = true;
}

Object* People::Get_Target()
{
	return target;
}

Inventory_Object* People::Get_Inventory(int index)
{
	if (index >= INVENTORY_SIZE)
		return nullptr;
	return inventory[index];
}

void People::Set_Inventory(int index, Inventory_Object* slot)
{
	inventory[index] = slot;
}

void People::Reset_Target()
{
	target = nullptr;
	way.clear();
}

void People::Reset_Sleep()
{
	sleep_count = max_sleep;
}

void People::Attack()
{
	Messenger* message = new Messenger;
	message->type = Types::attack_animal;
	message->attack_animal.target = (Animal*)target;
	message->attack_animal.damage = damage;
	Game::Get_Instance()->Send_Message(message);
}

void People::Take_Damage(int damage)
{
	heal_points -= damage;
}

void People::Enter_In_Building()
{
		in_building = true;
		int x = target->Get_Index().x + 1, y = target->Get_Index().y;
		Game::Get_Instance()->Set_Object(y, x, this);
		Game::Get_Instance()->Set_Object(Get_Index().y, Get_Index().x, nullptr);
		model.setSize(sf::Vector2f(0, 0));
}

void People::Exit_The_Building(sf::Vector2u index)
{
	in_building = false;
	int x = target->Get_Index().x + 1, y = target->Get_Index().y;
	Game::Get_Instance()->Set_Object(index.y, index.x, this);
	Game::Get_Instance()->Set_Object(y, x, target);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	sf::Vector2f pos = (sf::Vector2f)index;
	pos.x *= (WIN_WIDTH / MAP_WIDTH);
	pos.y *= (WIN_HEIGHT / MAP_HEIGHT);
	model.setPosition(pos);
}

int People::Get_Heal_Points()
{
	return heal_points;
}

int People::Get_Max_Heal_Points()
{
	return max_heal_point;
}

void People::Heal(int heal)
{
	heal_points += heal;
	if (heal_points > max_heal_point)
		heal_points = max_heal_point;
}

void People::Set_Target_Position(sf::Vector2u pos)
{
	target_pos = pos;
}

std::map<std::string, int> People::Get_Priority()
{
	return priority;
}



