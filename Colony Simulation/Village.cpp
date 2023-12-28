#include "Headers/Village.h"
#include "Headers/Main.h"
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
	timer = 0;
}

Village::~Village()
{
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
			int t = rand() % 4;
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
			}
			Game::Get_Instance()->Create_Object(type);
		}
	}
	else
		counter = 0;
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

	int x = rand() % 2;
	switch (x)
	{
	case 0:
		meat_type = "boar";
		break;
	case 1:
		meat_type = "chicken";
		break;
	}
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

bool Village::Can_Build(std::string type)
{
	return res_count[type] < res_max[type] and res_count["tree"] >= res_max["tree"] and res_count["rock"] >= res_max["rock"];
}
