#include <iostream>
#include "Headers/Game.h"
#include "Headers/Main.h"

sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Colony simulation");

int main()
{
	system("chcp 1251 > null");
	srand(time(NULL));
	Game* game = Game::Get_Instance();
	game->Start();
	Game::Destroy_Instance();
	return 0;
}