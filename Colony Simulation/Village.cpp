#include "Headers/Village.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include <map>

Village* Village::instance = nullptr;

Village::Village()
{
	res_count["people"] = 0;
	res_count["tree"] = 0;
	res_max["tree"] = 0;
	res_count["rock"] = 0;
	res_max["rock"] = 0;
	res_count["raw meat"] = 0;
	res_max["raw meat"] = 0;
	res_count["cooked meat"] = 0;
	res_max["cooked meat"] = 0;
	res_count["house"] = 0;
	res_max["house"] = 0;
	res_count["storage"] = 0;
	res_max["storage"] = 0;
	res_count["kitchen"] = 0;
	res_max["kitchen"] = 0;
	counter = 0;
	enemy_counter = 0;
	enemy_spawn_counter = 0;
	thief_counter = 0;
	thief_spawn_counter = 0;
	timer = 0;
}

Village::~Village()
{
}

void Village::Remove_Materials(int count, std::string type)
{
	for (auto s : storage_arr)
	{
		for (int i = 0; i < STORAGE_SIZE; i++)
			if (s->Get_Storage(i) != nullptr and s->Get_Storage(i)->Get_Type() == type)
			{
				if (s->Get_Storage(i)->Get_Count() >= count)
				{
					s->Get_Storage(i)->Change_Count(-count);
					Change_Count(-count, type);
					if (s->Get_Storage(i)->Get_Count() == 0)
					{
						s->Reset_Storage(i);
					}
					count = 0;
				}
				else
				{
					Change_Count(-s->Get_Storage(i)->Get_Count(), type);
					count = s->Get_Storage(i)->Change_Count(-s->Get_Storage(i)->Get_Count());
					s->Reset_Storage(i);
				}
			}
	}
	if (count > 0)
	{
		for (auto p : people_arr)
		{
			for (int i = 0; i < INVENTORY_SIZE; i++)
				if (p->Get_Inventory(i) != nullptr and p->Get_Inventory(i)->Get_Type() == type)
				{
					if (p->Get_Inventory(i)->Get_Count() >= count)
					{
						p->Get_Inventory(i)->Change_Count(-count);
						Change_Count(-count, type);
						count = 0;
						if (p->Get_Inventory(i)->Get_Count() == 0)
						{
							delete p->Get_Inventory(i);
							p->Set_Inventory(i, nullptr);
							/*Messenger* message = new Messenger;
							message->type = Types::empty_slot;
							message->empty_slot.index = i;
							message->empty_slot.people = p;
							message->empty_slot.slot = p->Get_Inventory(i);
							Game::Get_Instance()->Send_Message(message);*/
						}
					}
					else
					{
						Change_Count(-p->Get_Inventory(i)->Get_Count(), type);
						count = p->Get_Inventory(i)->Change_Count(-p->Get_Inventory(i)->Get_Count());
						delete p->Get_Inventory(i);
						p->Set_Inventory(i, nullptr);
						/*Messenger* message = new Messenger;
						message->type = Types::empty_slot;
						message->empty_slot.index = i;
						message->empty_slot.people = p;
						message->empty_slot.slot = p->Get_Inventory(i);
						Game::Get_Instance()->Send_Message(message);*/
					}
				}
		}
	}
}

Village* Village::Get_Instance()
{
	if (instance == nullptr)
		instance = new Village();
	return instance;
}

void Village::Destroy_Instance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Village::Change_Count(int d, std::string type)
{
	if (type == "people")
	{
		res_count["people"] += d;
		res_max["tree"] = 20 * res_count["people"];
		res_max["rock"] = 20 * res_count["people"];
		res_max["raw meat"] = 20 * res_count["people"];
		res_max["cooked meat"] = 20 * res_count["people"];
		res_max["house"] = res_count["people"];
		if (res_count["people"] % 3 == 0)
			res_max["storage"] = res_count["people"] / 3;
		if (res_count["people"] % 4 == 0)
			res_max["kitchen"] = res_count["people"] / 4;
		timer = 15 * res_count["people"];
	}
	else
		res_count[type] += d;
}

void Village::Show_Stat()
{
	std::cout << "Количество людей: " << res_count["people"] << std::endl;
	std::cout << "Количество дерева: " << res_count["tree"] << ". Максимум дерева: " << res_max["tree"] << std::endl;
	std::cout << "Количество камня: " << res_count["rock"] << ". Максимум камня: " << res_max["rock"] << std::endl;
	std::cout << "Количество сырого мяса: " << res_count["raw meat"] << ". Максимум сырого мяса: " << res_max["raw meat"] << std::endl;
	std::cout << "Количество приготовленного мяса: " << res_count["cooked meat"] << ". Максимум приготовленного мяса: " << res_max["cooked meat"] << std::endl;
	std::cout << "Количество домов: " << res_count["house"] << ". Максимум домов: " << res_max["house"] << std::endl;
	std::cout << "Количество складов: " << res_count["storage"] << ". Максимум складов: " << res_max["storage"] << std::endl;
	std::cout << "Количество кухонь: " << res_count["kitchen"] << ". Максимум кухонь: " << res_max["kitchen"] << std::endl << std::endl;
}

void Village::New_People()
{
	std::map<std::string, int> copy = res_count;
	bool flag = true;
	while (not copy.empty())
	{
		std::map<std::string, int>::iterator i = copy.begin();
		if (i->second < res_max[i->first])
			flag = false;
		copy.erase(i);
	}
	if (flag)
	{
		counter++;
		if (counter >= timer)
		{
			counter = 0;
			Change_Count(1, "people");
			std::string filename, type;
			int t = rand() % 5;
			switch (t)
			{
			case 0:
				type = "woodcutter";
				filename = "Images/People/Green_people.png";
				break;
			case 1:
				type = "miner";
				filename = "Images/People/Blue_people.png";
				break;
			case 2:
				type = "hunter";
				filename = "Images/People/Red_people.png";
				break;
			case 3:
				type = "builder";
				filename = "Images/People/Purple_people.png";
				break;
			case 4:
				type = "defender";
			}
			Game::Get_Instance()->Create_Object(type);
		}
	}
	else
		counter = 0;
}

void Village::Add_People(People* people)
{
	people_arr.push_back(people);
}

void Village::Delete_People(People* people)
{
	for (std::vector<People*>::iterator i = people_arr.begin(); i != people_arr.end(); i++)
	{
		if (*i == people)
		{
			people_arr.erase(i);
			break;
		}
	}
}

std::string Village::Find_Task(People* people)
{
	int max = INT_MIN;
	std::string meat_type, task;
	std::map<std::string, int> priority = people->Get_Priority(), coef;
	bool have_meat = false;
	for (int i = 0; i < INVENTORY_SIZE; i++)
		if (people->Get_Inventory(i) != nullptr)
			if (people->Get_Inventory(i)->Get_Type() == "raw meat")
				have_meat = true;
	if (res_count["tree"] < res_max["tree"])
		coef["tree"] = (res_max["tree"] - res_count["tree"]) * priority["woodcutter"];
	else
		coef["tree"] = priority["woodcutter"];

	if (res_count["rock"] < res_max["rock"])
		coef["rock"] = (res_max["rock"] - res_count["rock"]) * priority["miner"];
	else
		coef["rock"] = priority["miner"];
	coef["rock"]++;

	int x = rand() % 6;
	if (x < 3)
		meat_type = "chicken";
	else if (x < 5)
		meat_type = "boar";
	else
		meat_type = "dragon";

	if (res_count["raw meat"] < res_max["raw meat"])
		coef[meat_type] = (res_max["raw meat"] - res_count["raw meat"]) * priority["hunter"];
	else
		coef[meat_type] = priority["hunter"];

	if (res_count["cooked meat"] < res_max["cooked meat"] and have_meat and res_count["kitchen"] > 0)
		coef["kitchen"] = (res_max["cooked meat"] - res_count["cooked meat"]) * priority["hunter"];
	else if (have_meat and res_count["kitchen"] > 0)
		coef["kitchen"] = priority["hunter"];

	if (res_count["house"] < res_max["house"] and res_count["tree"] >= res_max["tree"] and res_count["rock"] >= res_max["rock"])
		coef["build house"] = priority["builder"] * 30;

	if (res_count["storage"] < res_max["storage"] and res_count["tree"] >= res_max["tree"] and res_count["rock"] >= res_max["rock"])
		coef["build storage"] = priority["builder"] * 10;

	if (res_count["kitchen"] < res_max["kitchen"] and res_count["tree"] >= res_max["tree"] and res_count["rock"] >= res_max["rock"])
		coef["build kitchen"] = priority["builder"] * 20;

	if (enemy_counter > 0)
		coef["enemy"] = enemy_counter * priority["defender"] * 10;

	else if (thief_counter > 0)
		coef["thief"] = thief_counter * priority["defender"] * 10;

	while (not coef.empty())
	{
		std::map<std::string, int>::iterator i = coef.begin();
		if (i->second >= max)
		{
			max = i->second;
			task = i->first;
		}
		coef.erase(i);
	}
	return task;
}

void Village::Add_Storage(Storage* storage)
{
	storage_arr.push_back(storage);
}

void Village::Delete_Storage(Storage* storage)
{
	for (std::vector<Storage*>::iterator i = storage_arr.begin(); i != storage_arr.end(); i++)
	{
		if (*i == storage)
		{
			storage_arr.erase(i);
			break;
		}
	}
}

void Village::Remove_Materials(std::pair<int, int> resources)
{
	Remove_Materials(resources.first, "tree");
	Remove_Materials(resources.second, "rock");
}

bool Village::Can_Build(std::string type)
{
	return res_count[type] < res_max[type] and res_count["tree"] >= res_max["tree"] and res_count["rock"] >= res_max["rock"];
}

void Village::Spawn_Enemy()
{
	enemy_spawn_counter++;
	if (enemy_spawn_counter >= 50)
	{
		enemy_spawn_counter = 0;
		sf::Vector2f pos;
		Enemy* enemy;
		if (rand() % 4 == 0)
		{
			int enemy_count = rand() % res_count["people"] + 1, x, y;
			enemy_counter += enemy_count;
			for (int i = 0; i < enemy_count; i++)
			{
				do
				{
					switch (rand() % 4)
					{
					case 0:
						x = rand() % (MAP_WIDTH - 2) + 1;
						y = 1;
						break;
					case 1:
						x = rand() % (MAP_WIDTH - 2) + 1;
						y = MAP_HEIGHT - 2;
						break;
					case 2:
						x = MAP_WIDTH - 2;
						y = rand() % (MAP_HEIGHT - 2) + 1;
						break;
					case 3:
						x = 1;
						y = rand() % (MAP_HEIGHT - 2) + 1;
						break;
					}
				} while (Game::Get_Instance()->Get_Object(y, x) != nullptr);
				pos.x = x * WIN_WIDTH / MAP_WIDTH;
				pos.y = y * WIN_HEIGHT / MAP_HEIGHT;
				enemy = new Enemy(pos, Game::Get_Instance()->Get_Texture("enemy"));
				Game::Get_Instance()->Set_Object(y, x, enemy);
			}
		}
	}
}

void Village::Spawn_Thief()
{
	if (not storage_arr.empty())
	{
		thief_spawn_counter++;
		if (thief_spawn_counter >= 50)
		{
			thief_counter++;
			thief_spawn_counter = 0;
			sf::Vector2f pos;
			Thief* thief;
			if (rand() % 4 == 0)
			{
				int x, y = MAP_HEIGHT - 2;
				do
				{
					x = rand() % (MAP_WIDTH - 2) + 1;;
				} while (Game::Get_Instance()->Get_Object(y, x) != nullptr and Game::Get_Instance()->Get_Object(y + 1, x) != nullptr);
				pos.x = x * WIN_WIDTH / MAP_WIDTH;
				pos.y = y * WIN_HEIGHT / MAP_HEIGHT;
				thief = new Thief(pos, Game::Get_Instance()->Get_Texture("thief"));
				Game::Get_Instance()->Set_Object(y, x, thief);
			}
		}
	}
}

void Village::Enemy_Died()
{
	enemy_counter--;
}

void Village::Thief_Died()
{
	thief_counter--;
}


