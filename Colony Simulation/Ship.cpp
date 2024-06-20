#include "Headers/Ship.h"
#include "Headers/Main.h"

Ship::Ship(sf::Vector2f pos, sf::Texture* tex)
{
	is_alive = false;
	this->type = "ship";
	texture = tex;
	sf::Vector2u vector = texture->getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(texture);
	text_pos.left = vector.x / 4;
	text_pos.top = 0;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y;
	model.setTextureRect(text_pos);
}

Ship::~Ship()
{
}

void Ship::Show()
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
	image.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4, WIN_HEIGHT / 9 * 5));
	image.setTexture(texture);
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

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9));
			text.setString(type);
			window.draw(text);

			window.draw(image);
			window.display();
		}
	}
}
