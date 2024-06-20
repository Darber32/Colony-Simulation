#pragma once
#include "Animal.h"

class Dragon: public Animal
{
private:
	int damage, area;
	Object* target;
    std::vector<sf::Vector2u> way;
    sf::Vector2u target_pos;
    void Move();
    void Attack();
    void Interact();
    void Change_Texture(int type);
    void Find_Target_In_Area();
    void Find_Shortest_Way();
public:
    Dragon(sf::Vector2f pos, sf::Texture* tex);
    ~Dragon();
    void Update();
    void Show();
    void Take_Damage(int damage);
    int Get_Damage();
    int Interact_With_Dragon(Dragon* dragon, int damage);
};

