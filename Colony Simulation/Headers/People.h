#pragma once
#define INVENTORY_SIZE 5
#include <vector>
#include <map>
#include "Object.h"
#include "Inventory_Object.h"

class Interactable_Object;

class People : public Object
{
private:
	std::string task_type;
	Inventory_Object* inventory[INVENTORY_SIZE];
	int heal_points, max_heal_point, hunger_points, max_hunger_points, damage, sleep_count, max_sleep, build_time, build_max;
	std::map<std::string, int> priority;
	bool in_building;
	std::vector<sf::Vector2u> way;
	Interactable_Object * target;
	sf::Vector2u target_pos;
	void Move();
	void Walk();
	void Find_Nearest_Object(std::string type);
	void Find_Place_For_Building(std::string type);
	void Change_Texture(int type);
	void Eat();
	bool Chech_Place(int i, int j);
	void Way_For_Building(int map[MAP_HEIGHT][MAP_WIDTH]);
	void Way_For_Object(int map[MAP_HEIGHT][MAP_WIDTH]);
public:
	void Find_Shortest_Way();
	People(sf::Vector2f pos, std::string type);
	~People();
	void Update();
	void Was_Update();
	Object* Get_Target();
	Inventory_Object* Get_Inventory(int index);
	void Set_Inventory(int index, Inventory_Object* slot);
	void Reset_Target();
	void Attack();
	void Take_Damage(int damage);
	void Enter_In_Building();
	void Exit_The_Building(sf::Vector2u index);
	int Get_Heal_Points();
	int Get_Max_Heal_Points();
	void Heal(int heal);
	void Set_Target_Position(sf::Vector2u pos);
	std::map<std::string, int> Get_Priority();
	void Show();
	void Reset_Sleep();
};

