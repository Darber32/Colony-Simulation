#include "Headers/Boar.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"

Boar::Boar(sf::Vector2f pos, sf::Texture* tex)
{
	meat_count = rand() % 41 + 20;
	type = "boar";
	hp = 10;
	damage = 1;
	was_attacked = false;
	texture = tex;
	sf::Vector2u vector = texture->getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(texture);
	text_pos.left = 0;
	text_pos.top = 0;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 8;
	model.setTextureRect(text_pos);
}

Boar::~Boar()
{
}

void Boar::Move()
{
	int need_to_move = rand() % 2;
	if (need_to_move == 0)
	{
		sf::Vector2u pos = Get_Index();
		int new_x, new_y, direction;
		do
		{
			do
			{
				new_x = pos.x;
				new_y = pos.y;
				direction = rand() % 4;
				switch (direction)
				{
				case 0:
					new_x++;
					Change_Texture(2);
					break;
				case 1:
					new_x--;
					Change_Texture(3);
					break;
				case 2:
					new_y++;
					Change_Texture(0);
					break;
				case 3:
					new_y--;
					Change_Texture(1);
					break;
				}
			} while (new_x < 1 or new_x > MAP_WIDTH - 2 or new_y < 1 or new_y > MAP_HEIGHT - 2);
		} while (Game::Get_Instance()->Get_Object(new_y, new_x) != nullptr);
		Game::Get_Instance()->Set_Object(new_y, new_x, this);
		Game::Get_Instance()->Set_Object(pos.y, pos.x, nullptr);
		pos.x = new_x * (WIN_WIDTH / MAP_WIDTH);
		pos.y = new_y * (WIN_HEIGHT / MAP_HEIGHT);
		model.setPosition((sf::Vector2f)pos);
		was_update = true;
	}
}

void Boar::Attack()
{
	if (hp > 0)
	{
		Messenger* message = new Messenger;
		message->type = Types::attack_people;
		message->attack_people.target = who_target;
		message->attack_people.damage = damage;
		Game::Get_Instance()->Send_Message(message);
	}
}

void Boar::Update()
{
	if (not was_update)
	{
		if (hp > 0)
		{
			if (not was_attacked)
			{
				if (who_target != nullptr and who_target->Get_Heal_Points() <= 0)
					who_target = nullptr;
				if (who_target != nullptr)
				{
					int x = Get_Index().x, y = Get_Index().y;
					for (int i = y - 1; i <= y + 1; i++)
						for (int j = x - 1; j <= x + 1; j++)
						{
							if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
							{
								Object* object = Game::Get_Instance()->Get_Object(i, j);
								if (object != nullptr and object == who_target)
									Interact();
							}
						}
				}
				if (not was_update)
					Move();
			}
			else
				was_attacked = false;
		}
		else
			Death();
	}
}

void Boar::Show()
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

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 2));
			text.setString("Heal points:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9 * 2));
			text.setString(std::to_string(hp));
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 3));
			text.setString("Damage:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9 * 3));
			text.setString(std::to_string(damage));
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 4));
			text.setString("Count of meat:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9 * 4));
			text.setString(std::to_string(meat_count));
			window.draw(text);

			window.draw(image);
			window.display();
		}
	}
}

int Boar::Interact_With_Dragon(Dragon* dragon, int damage)
{
	hp -= damage;
	if (hp <= 0)
		is_alive = false;
	was_update = true;
	was_attacked = true;
	return this->damage;
}

void Boar::Interact()
{
	who_target->Attack();
	Attack();
	was_update = true;
}

void Boar::Change_Texture(int type)
{
	sf::Vector2u vector = texture->getSize();
	text_pos.left = 0;
	text_pos.top = vector.y / 8 * type;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 8;
	model.setTextureRect(text_pos);
}
