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
	Village::Get_Instance()->Remove_Materials(cost);
	texture.loadFromFile("Images/Buildings/Wood/Houses.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	text_pos.left = vector.x / 3 * (rand() % 3);
	text_pos.top = vector.y / 4 * (rand() % 4);
	text_pos.width = vector.x / 3;
	text_pos.height = vector.y / 4;
	model.setTextureRect(text_pos);
}

void House::Show()
{
	sf::Event event;
	bool flag = true;
	sf::Texture exit_text, back_text;
	exit_text.loadFromFile("Images/Cross.png");
	back_text.loadFromFile("Images/Ground/Back.png");
	sf::RectangleShape exit, image, back;

	exit.setSize(sf::Vector2f(WIN_WIDTH / 30, WIN_HEIGHT / 30));
	exit.setPosition(sf::Vector2f(WIN_WIDTH / 30 * 29, 0));
	exit.setFillColor(sf::Color::Red);
	exit.setTexture(&exit_text);

	image.setSize(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9));
	image.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4, WIN_HEIGHT / 9 * 4));
	image.setTexture(&texture);
	image.setTextureRect(text_pos);

	back.setSize(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));
	back.setPosition(sf::Vector2f(0, 0));
	back.setTexture(&back_text);

	sf::Font font;
	font.loadFromFile("consolai.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(40);
	text.setFillColor(sf::Color::White);
	while (flag)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				flag = false;
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
					if (event.mouseButton.x >= (WIN_WIDTH / 30 * 29) and event.mouseButton.y <= WIN_HEIGHT / 30)
						flag = false;
				break;
			}

		}
		if (flag)
		{
			window.clear(sf::Color::Black);
			window.draw(back);
			window.draw(exit);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9));
			text.setString("Type:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 6, WIN_HEIGHT / 9));
			text.setString(type);
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 2));
			text.setString("Anybody in building?");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 6, WIN_HEIGHT / 9 * 2));
			if (is_anybody_inside)
				text.setString("yes");
			else
				text.setString("no");
			window.draw(text);

			window.draw(image);
			window.display();
		}
	}
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
