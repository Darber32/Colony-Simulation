#include "Headers/House.h"
#include "Headers/Main.h"
#include "Headers/Resources.h"
#include "Headers/Village.h"

House::House(sf::Vector2f pos)
{
	Village::Get_Instance()->Change_Count(1, "house");
	type = "house";
	sleep_count = 0;
	max_sleep = 8;
	cost = std::pair<int, int>(30, 30);
	texture.loadFromFile("Images/Buildings/Wood/Houses.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(vector.x / 3 * (rand() % 3), vector.y / 4 * (rand() % 4), vector.x / 3, vector.y / 4));
}

void House::Interact()
{
	if (not is_anybody_inside)
		Enter();	
	sleep_count++;
	was_update = true;
	if (sleep_count >= max_sleep)
	{
		sleep_count = 0;
		who_target->Heal(who_target->Get_Max_Heal_Points());
		who_target->Reset_Sleep();
		Exit();
	}

}
