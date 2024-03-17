#include "Headers/Chicken.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"

Chicken::Chicken(sf::Vector2f pos)
{
	meat_count = rand() % 31 + 10;
	run_count = 0;
	max_run = 5;
	need_run = false;
	type = "chicken";
	hp = 5;
	texture.loadFromFile("Images/Animals/Chicken.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(&texture);
	text_pos.left = 0;
	text_pos.top = 0;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 4;
	model.setTextureRect(text_pos);
}

Chicken::~Chicken()
{
}

void Chicken::Move()
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
				direction = rand() % 8;
				switch (direction)
				{
				case 0:
					new_x++;
					Change_Texture(3);
					break;
				case 1:
					new_x--;
					Change_Texture(2);
					break;
				case 2:
					new_y++;
					Change_Texture(0);
					break;
				case 3:
					new_y--;
					Change_Texture(1);
					break;
				case 4:
					new_x++;
					new_y++;
					Change_Texture(0);
					break;
				case 5:
					new_x++;
					new_y--;
					Change_Texture(1);
					break;
				case 6:
					new_x--;
					new_y++;
					Change_Texture(0);
					break;
				case 7:
					new_x--;
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
		Messenger* message = new Messenger;
		message->type = Types::move;
		message->move.animal = this;
		message->move.target = who_target;
		message->move.pos = pos;
		Game::Get_Instance()->Send_Message(message);
		was_update = true;
		run_count++;
	}
}

void Chicken::Run()
{
	if (need_run)
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
					new_x+= 2;
					Change_Texture(3);
					break;
				case 1:
					new_x-= 2;
					Change_Texture(2);
					break;
				case 2:
					new_y+= 2;
					Change_Texture(0);
					break;
				case 3:
					new_y-= 2;
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
		Messenger* message = new Messenger;
		message->type = Types::move;
		message->move.animal = this;
		message->move.target = who_target;
		message->move.pos = pos;
		Game::Get_Instance()->Send_Message(message);
		was_update = true;
		run_count++;
		if (run_count >= max_run)
		{
			run_count = 0;
			need_run = false;
		}
	}
}

void Chicken::Update()
{
	if (not was_update)
	{
		if (hp > 0)
		{
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
			if (hp > 0)
			{
				if (not need_run)
				{
					Move();
				}
				else
				{
					Run();
				}
			}
		}
		else
			Death();
	}
}

void Chicken::Show()
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
			text.setString("Count of meat:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9 * 3));
			text.setString(std::to_string(meat_count));
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 4));
			text.setString("Run or move: ");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 9 * 4));
			if (need_run)
				text.setString("run");
			else
				text.setString("move");
			window.draw(text);

			window.draw(image);
			window.display();
		}
	}
}

void Chicken::Interact()
{
	who_target->Attack();
	need_run = true;
}

void Chicken::Change_Texture(int type)
{
	sf::Vector2u vector = texture.getSize();
	text_pos.left = 0;
	text_pos.top = vector.y / 4 * type;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 4;
	model.setTextureRect(sf::IntRect(0, vector.y / 4 * type, vector.x / 4, vector.y / 4));
}
