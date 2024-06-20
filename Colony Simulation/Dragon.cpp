#include "Headers/Dragon.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"

void Dragon::Move()
{
	if (not was_update)
	{
		if (target == nullptr)
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
		else
		{
			sf::Vector2u pos = Get_Index();
			sf::Vector2u new_pos = way[way.size() - 1];
			if (Game::Get_Instance()->Get_Object(new_pos.y, new_pos.x) != nullptr or Game::Get_Instance()->Get_Object(target_pos.y, target_pos.x) != target)
			{
				Find_Shortest_Way();
				if (not way.empty())
					new_pos = way[way.size() - 1];
			}
			if (not way.empty())
			{
				way.pop_back();
				if (pos.y > new_pos.y)
					Change_Texture(1);
				else if (pos.y < new_pos.y)
					Change_Texture(0);
				else if (pos.x > new_pos.x)
					Change_Texture(2);
				else if (pos.x < new_pos.x)
					Change_Texture(3);
				Game::Get_Instance()->Set_Object(new_pos.y, new_pos.x, this);
				Game::Get_Instance()->Set_Object(pos.y, pos.x, nullptr);
				pos.x = new_pos.x * (WIN_WIDTH / MAP_WIDTH);
				pos.y = new_pos.y * (WIN_HEIGHT / MAP_HEIGHT);
				model.setPosition((sf::Vector2f)pos);
				was_update = true;
			}
		}
	}
}

void Dragon::Attack()
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

void Dragon::Interact()
{
	Attack();
	who_target->Attack();
	was_update = true;
}

void Dragon::Change_Texture(int type)
{
	sf::Vector2u vector = texture->getSize();
	text_pos.left = 0;
	text_pos.top = vector.y / 8 * type;
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 8;
	model.setTextureRect(text_pos);
}

void Dragon::Find_Target_In_Area()
{
	int x = Get_Index().x, y = Get_Index().y;
	bool flag = true;
	for (int i = y - area; i <= y + area and flag; i++)
		for (int j = x - area; j <= x + area; j++)
		{
			if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
			{
				Object* object = Game::Get_Instance()->Get_Object(i, j);
				if (object != nullptr and object->Is_Alive() and object != this and object->Get_Type() != type)
				{
					target = object;
					target_pos = (sf::Vector2u)target->Get_Pos();
					flag = false;
					break;
				}
			}
		}
}

void Dragon::Find_Shortest_Way()
{
	way.clear();
	bool flag = true;
	int map[MAP_HEIGHT][MAP_WIDTH];
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			map[i][j] = 99;
	std::vector<sf::Vector2u> wave, old_wave;
	int x = Get_Index().x, y = Get_Index().y;
	old_wave.push_back(sf::Vector2u(x, y));
	int step = 0;
	map[y][x] = step;
	while (not old_wave.empty() and flag)
	{
		step++;
		wave.clear();
		for (std::vector<sf::Vector2u>::iterator i = old_wave.begin(); i != old_wave.end(); i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int new_x = i->x, new_y = i->y;
				switch (j)
				{
				case 0:
					if (new_x != MAP_WIDTH - 1)
						if (Game::Get_Instance()->Get_Object(new_y, new_x + 1) == nullptr and map[new_y][new_x + 1] > step)
							new_x++;
					break;
				case 1:
					if (new_x != 0)
						if (Game::Get_Instance()->Get_Object(new_y, new_x - 1) == nullptr and map[new_y][new_x - 1] > step)
							new_x--;
					break;
				case 2:
					if (new_y != MAP_HEIGHT - 1)
						if (Game::Get_Instance()->Get_Object(new_y + 1, new_x) == nullptr and map[new_y + 1][new_x] > step)
							new_y++;
					break;
				case 3:
					if (new_y != 0)
						if (Game::Get_Instance()->Get_Object(new_y - 1, new_x) == nullptr and map[new_y - 1][new_x] > step)
							new_y--;
					break;
				}
				if (flag and (new_x != i->x or new_y != i->y))
				{
					wave.push_back(sf::Vector2u(new_x, new_y));
					map[new_y][new_x] = step;
				}
			}
		}
		old_wave = wave;
	}
	wave.clear();
	old_wave.clear();
	x = target->Get_Index().x, y = target->Get_Index().y;
	int min = INT_MAX, min_x, min_y;
	while (map[y][x] != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			int new_x = x, new_y = y;
			switch (i)
			{
			case 0:
				if (x != MAP_WIDTH - 1)
					new_x++;
				break;
			case 1:
				if (x != 0)
					new_x--;
				break;
			case 2:
				if (y != MAP_HEIGHT - 1)
					new_y++;
				break;
			case 3:
				if (y != 0)
					new_y--;
				break;
			}
			if (x != new_x or y != new_y)
				if (map[new_y][new_x] < min)
				{
					min = map[new_y][new_x];
					min_x = new_x;
					min_y = new_y;
				}
		}
		x = min_x;
		y = min_y;
		if (map[y][x] != 0)
			way.push_back(sf::Vector2u(x, y));
	}
}

Dragon::Dragon(sf::Vector2f pos, sf::Texture* tex)
{
	meat_count = rand() % 60 + 20;
	type = "dragon";
	hp = 8;
	area = 2;
	damage = 3;
	target = nullptr;
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

Dragon::~Dragon()
{
}

void Dragon::Update()
{
	if (hp > 0)
	{
		if (target == nullptr)
		{
			Find_Target_In_Area();
			if (target != nullptr)
				Find_Shortest_Way();
		}
		if (target == nullptr or (target != nullptr and not way.empty()))
			Move();
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
						{
							if (who_target->Get_Target() != this)
							{
								who_target->Reset_Target();
								who_target->Set_Target(this);
							}
							Interact();
						}
					}
				}
		}
		if (target != nullptr)
		{
			if (not target->Is_Alive())
				target = nullptr;
			int x = Get_Index().x, y = Get_Index().y;
			for (int i = y - 1; i <= y + 1; i++)
				for (int j = x - 1; j <= x + 1; j++)
				{
					if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
					{
						Object* object = Game::Get_Instance()->Get_Object(i, j);
						if (object != nullptr and object == target)
						{
							if (target->Get_Type() == "people")
								who_target = (People*)target;
							hp -= object->Interact_With_Dragon(this, damage);
							if (not object->Is_Alive())
								target = nullptr;
						}
					}
				}
		}
	}
	else
		Death();
}

void Dragon::Show()
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

void Dragon::Take_Damage(int damage)
{
	hp -= damage;
}

int Dragon::Get_Damage()
{
	return damage;
}

int Dragon::Interact_With_Dragon(Dragon* dragon, int damage)
{
	return 0;
}
