#pragma once
#include <iostream>
#include "People.h"
#include <map>

class Village
{
private:
	static Village* instance;
	std::map<std::string, int> res_count, res_max;
	int counter, timer;
	Village();
	~Village();
public:
	static Village* Get_Instance();
	static void Destroy_Instance();
	void Change_Count(int d, std::string type);
	void Show_Stat();
	void New_People();
	std::string Find_Task(People * people);
	bool Can_Build(std::string type);
};

