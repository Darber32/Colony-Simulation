#pragma once
#include "Interactable_Object.h"
#include "Ship.h"
#include "Storage.h"

class Thief : public Interactable_Object
{
private:
	int hp, damage, walk_counter, interaction_counter;
	bool is_full, was_attacked;
	sf::Vector2f ship_pos;
	Storage* storage;
	Ship* ship;
	Inventory_Object* inventory[STORAGE_SIZE];
	std::vector<sf::Vector2u> way;
	sf::Vector2u storage_pos;
	void Move();
	void Attack();
	void Interact();
	void Interact_With_Storage();
	void Change_Texture(int type);
	void Death();
	void Find_Shortest_Way_To_Ship();
	void Find_Nearest_Storage();
	void Find_Shortest_Way_To_Storage();
	void Leave();
public:
	Thief(sf::Vector2f pos, sf::Texture* tex);
	~Thief();
	void Update();
	void Show();
	void Take_Damage(int damage);
	int Interact_With_Dragon(Dragon* dragon, int damage);
};

