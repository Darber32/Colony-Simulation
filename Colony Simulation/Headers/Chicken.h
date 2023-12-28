#pragma once
#include "Animal.h"
class Chicken : public Animal
{
private:
    int run_count, max_run;
    bool need_run;
    void Move();
    void Run();
    void Interact();
    void Change_Texture(int type);
public:
    Chicken(sf::Vector2f pos);
    ~Chicken();
    void Update();
};

