#pragma once
#include "Interactable_Object.h"
class Enemy: public Interactable_Object
{
protected:
	int hp, damage;
	bool was_attacked;
	std::vector<sf::Vector2u> way;
	sf::Vector2u target_pos;
	void Move();
	void Walk();
	void Attack();
	void Interact();
	void Change_Texture(int type);
	void Death();
	void Find_Nearest_People();
	void Find_Shortest_Way();
public:
	Enemy(sf::Vector2f pos, sf::Texture* tex);
	~Enemy();
	void Update();
	void Show();
	void Take_Damage(int damage);
	int Interact_With_Dragon(Dragon* dragon, int damage);
};

