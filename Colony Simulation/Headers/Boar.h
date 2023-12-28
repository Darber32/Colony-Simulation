#pragma once
#include "Animal.h"
class Boar :
    public Animal
{
private:
    int damage;
    void Move();
    void Attack();
    void Interact();
    void Change_Texture(int type);
public:
    Boar(sf::Vector2f pos);
    ~Boar();
    void Update();
};

