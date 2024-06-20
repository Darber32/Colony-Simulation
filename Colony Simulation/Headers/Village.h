#pragma once
#include <iostream>
#include "People.h"
#include "Storage.h"
#include "Enemy.h"
#include "Thief.h"
#include <map>
#include <vector>

class Village
{
private:
	static Village* instance;
	std::map<std::string, int> res_count, res_max;
	std::vector<People*> people_arr;
	std::vector<Storage*> storage_arr;
	int counter, enemy_counter, thief_counter, enemy_spawn_counter, thief_spawn_counter, timer;
	Village();
	~Village();
	void Remove_Materials(int count, std::string type);
public:
	static Village* Get_Instance();
	static void Destroy_Instance();
	void Change_Count(int d, std::string type);
	void Show_Stat();
	void New_People();
	void Add_People(People* people);
	void Delete_People(People * people);
	std::string Find_Task(People * people);
	void Add_Storage(Storage* storage);
	void Delete_Storage(Storage* storage);
	void Remove_Materials(std::pair<int, int> resources);
	bool Can_Build(std::string type);
	void Spawn_Enemy();
	void Spawn_Thief();
	void Enemy_Died();
	void Thief_Died();
};

