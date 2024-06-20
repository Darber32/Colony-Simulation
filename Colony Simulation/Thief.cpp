#include "Headers/Thief.h"
#include "Headers/Main.h"
#include "Headers/Village.h"
#include "Headers/Messenger.h"

Thief::Thief(sf::Vector2f pos, sf::Texture* tex)
{
	type = "thief";
	hp = 8;
	damage = 1;
	walk_counter = 0;
	interaction_counter = 0;
	was_update = true;
	is_full = false;
	is_alive = true;
	is_interactable = true;
	was_attacked = false;
	storage = nullptr;
	ship_pos = pos;
	ship_pos.y += WIN_HEIGHT / MAP_HEIGHT;
	ship = new Ship(ship_pos, Game::Get_Instance()->Get_Texture("ship"));
	Game::Get_Instance()->Set_Object(ship_pos.y / (WIN_HEIGHT / MAP_HEIGHT), ship_pos.x / (WIN_WIDTH / MAP_WIDTH), ship);
	texture = tex;
	sf::Vector2u vector = texture->getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(texture);
	text_pos.left = 0;
	text_pos.top = 0;
	text_pos.width = vector.x / 5;
	text_pos.height = vector.y / 12;
	model.setTextureRect(text_pos);
	for (int i = 0; i < STORAGE_SIZE; i++)
		inventory[i] = nullptr;
}

Thief::~Thief()
{
	Village::Get_Instance()->Thief_Died();
	delete ship;
	if (who_target != nullptr and who_target->Get_Target() == this)
		who_target->Reset_Target();
	Game::Get_Instance()->Set_Object(ship_pos.y / (WIN_HEIGHT / MAP_HEIGHT), ship_pos.x / (WIN_WIDTH / MAP_WIDTH), nullptr);
	for (int i = 0; i < STORAGE_SIZE; i++)
		if (inventory[i] != nullptr)
		{
			Village::Get_Instance()->Change_Count(-inventory[i]->Get_Count(), inventory[i]->Get_Type());
			delete inventory[i];
			inventory[i] = nullptr;
		}
}

void Thief::Update()
{
	if (not was_update)
	{
		if (hp > 0)
		{
			if (not was_attacked)
			{
				if (not is_full)
				{
					if (storage == nullptr)
					{
						Find_Nearest_Storage();
						Find_Shortest_Way_To_Storage();
					}
					if (who_target != nullptr)
					{
						int x = Get_Index().x, y = Get_Index().y;
						for (int i = y - 1; i <= y + 1; i++)
						{
							for (int j = x - 1; j <= x + 1; j++)
							{
								if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
								{
									Object* object = Game::Get_Instance()->Get_Object(i, j);
									if (object != nullptr and object == who_target)
									{
										Interact();
									}
								}
							}
						}
					}
					if (not way.empty())
						Move();
					else
					{
						int x = Get_Index().x, y = Get_Index().y;
						for (int i = y - 1; i <= y + 1; i++)
						{
							for (int j = x - 1; j <= x + 1; j++)
							{
								if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
								{
									Object* object = Game::Get_Instance()->Get_Object(i, j);
									if (object != nullptr and object == storage)
									{
										Interact_With_Storage();
									}
								}
							}
						}
					}
				}
				else
				{
					if (who_target != nullptr)
					{
						int x = Get_Index().x, y = Get_Index().y;
						for (int i = y - 1; i <= y + 1; i++)
						{
							for (int j = x - 1; j <= x + 1; j++)
							{
								if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
								{
									Object* object = Game::Get_Instance()->Get_Object(i, j);
									if (object != nullptr and object == who_target)
									{
										Interact();
									}
								}
							}
						}
					}
					if (way.empty())
					{
						bool is_active = true;
						int x = Get_Index().x, y = Get_Index().y;
						for (int i = y - 1; i <= y + 1; i++)
						{
							for (int j = x - 1; j <= x + 1; j++)
							{
								if (i > 0 and i <= MAP_HEIGHT - 1 and j > 0 and j <= MAP_WIDTH - 1)
								{
									Object* object = Game::Get_Instance()->Get_Object(i, j);
									if (object != nullptr and object == ship)
									{
										is_active = false;
										Leave();
									}
								}
							}
						}
						if (is_active)
							Find_Shortest_Way_To_Ship();
					}
					else
						Move();
				}
			}
		}
		else
		{
			Death();
		}
	}
}

void Thief::Show()
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
	image.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4, WIN_HEIGHT / 9 * 8 - WIN_HEIGHT / MAP_HEIGHT));
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

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15));
			text.setString("Type:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4 + WIN_WIDTH / 18, WIN_HEIGHT / 15));
			text.setString(type);
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15 * 2));
			text.setString("Heal points:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4 + WIN_WIDTH / 18, WIN_HEIGHT / 15 * 2));
			text.setString(std::to_string(hp));
			window.draw(text);

			for (int i = 0; i < STORAGE_SIZE; i++)
			{
				text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15 * (3 + i)));
				text.setString("Slot number " + std::to_string(i + 1) + ":");
				window.draw(text);

				text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 4 + WIN_WIDTH / 18, WIN_HEIGHT / 15 * (3 + i)));
				if (inventory[i] != nullptr)
					text.setString(inventory[i]->Get_Type() + " count: " + std::to_string(inventory[i]->Get_Count()));
				else
					text.setString("empty");
				window.draw(text);
			}

			window.draw(image);
			window.display();
		}
	}
}

void Thief::Take_Damage(int damage)
{
	hp -= damage;
}

int Thief::Interact_With_Dragon(Dragon* dragon, int damage)
{
	hp -= damage;
	if (hp <= 0)
		is_alive = false;
	was_update = true;
	was_attacked = true;
	return this->damage;
}

void Thief::Move()
{
	if (not was_update)
	{
		if (not is_full)
		{
			sf::Vector2u pos = Get_Index();
			sf::Vector2u new_pos = way[way.size() - 1];
			if (Game::Get_Instance()->Get_Object(new_pos.y, new_pos.x) != nullptr)
			{
				Find_Shortest_Way_To_Storage();
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
					Change_Texture(3);
				else if (pos.x < new_pos.x)
					Change_Texture(2);
				Game::Get_Instance()->Set_Object(new_pos.y, new_pos.x, this);
				Game::Get_Instance()->Set_Object(pos.y, pos.x, nullptr);
				pos.x = new_pos.x * (WIN_WIDTH / MAP_WIDTH);
				pos.y = new_pos.y * (WIN_HEIGHT / MAP_HEIGHT);
				model.setPosition((sf::Vector2f)pos);
				was_update = true;
			}
		}
		else
		{
			walk_counter++;
			if (walk_counter >= 2)
			{
				walk_counter = 0;
				sf::Vector2u pos = Get_Index();
				sf::Vector2u new_pos = way[way.size() - 1];
				if (Game::Get_Instance()->Get_Object(new_pos.y, new_pos.x) != nullptr)
				{
					Find_Shortest_Way_To_Ship();
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
						Change_Texture(3);
					else if (pos.x < new_pos.x)
						Change_Texture(2);
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
}

void Thief::Attack()
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

void Thief::Interact()
{
	Attack();
	who_target->Attack_Thief();
	was_update = true;
}

void Thief::Interact_With_Storage()
{
	interaction_counter++;
	if (interaction_counter >= 5)
	{
		for (int i = 0; i < STORAGE_SIZE; i++)
		{
			if (storage->Get_Storage(i) != nullptr)
			{
				inventory[i] = storage->Get_Storage(i);
				storage->Reset_Storage(i);
			}
		}
		is_full = true;
	}
}

void Thief::Change_Texture(int type)
{
	sf::Vector2u vector = texture->getSize();
	text_pos.left = 0;
	text_pos.top = vector.y / 12 * type;
	text_pos.width = vector.x / 5;
	text_pos.height = vector.y / 12;
	model.setTextureRect(text_pos);
}

void Thief::Death()
{
	is_alive = false;
	if (who_target != nullptr)
		who_target->Reset_Target();
	if (storage != nullptr)
	{
		for (int i = 0; i < STORAGE_SIZE; i++)
		{
			if (inventory[i] != nullptr)
			{
				for (int j = 0; j < STORAGE_SIZE; j++)
				{
					if (storage->Get_Storage(j) == nullptr)
					{
						storage->Set_Storage(j, inventory[i]);
						inventory[i] = nullptr;
						break;
					}
				}
			}
		}
		for (int i = 0; i < STORAGE_SIZE; i++)
			if (inventory[i] != nullptr)
			{
				Village::Get_Instance()->Change_Count(-inventory[i]->Get_Count(), inventory[i]->Get_Type());
				delete inventory[i];
				inventory[i] = nullptr;
			}
	}
	Messenger* message = new Messenger;
	message->type = Types::death;
	message->death.dying = this;
	Game::Get_Instance()->Send_Message(message);
}

void Thief::Find_Shortest_Way_To_Ship()
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
	x = ship->Get_Index().x, y = ship->Get_Index().y - 1;
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

void Thief::Find_Nearest_Storage()
{
	int my_i = Get_Index().y, my_j = Get_Index().x;
	std::vector<Storage*> arr;
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			Object* object = Game::Get_Instance()->Get_Object(i, j);
			if (object != nullptr and object->Get_Type() == "storage")
				arr.push_back((Storage*)object);
		}
	int min = INT_MAX, cur = 0;
	for (int i = 0; i < arr.size(); i++)
	{
		int x = abs(my_j - (int)arr[i]->Get_Index().x);
		int y = abs(my_i - (int)arr[i]->Get_Index().y);
		cur = x + y;
		if (cur < min)
		{
			min = cur;
			storage = arr[i];
			storage_pos = (sf::Vector2u)storage->Get_Pos();
		}
	}
}

void Thief::Find_Shortest_Way_To_Storage()
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
	if (storage != nullptr)
	{
		x = storage->Get_Index().x, y = storage->Get_Index().y;
		if (Get_Index().x > x)
			x++;
		if (Get_Index().y > y)
			y++;
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
}

void Thief::Leave()
{
	for (int i = 0; i < STORAGE_SIZE; i++)
		if (inventory[i] != nullptr)
		{
			Village::Get_Instance()->Change_Count(-inventory[i]->Get_Count(), inventory[i]->Get_Type());
			delete inventory[i];
			inventory[i] = nullptr;
		}
	Death();
}