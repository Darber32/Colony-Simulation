#pragma once
#include "Animal.h"
class Boar :
    public Animal
{
private:
    int damage;
    bool was_attacked;
    void Move();
    void Attack();
    void Interact();
    void Change_Texture(int type);
public:
    Boar(sf::Vector2f pos, sf::Texture* tex);
    ~Boar();
    void Update();
    void Show();
    int Interact_With_Dragon(Dragon* dragon, int damage);
};

