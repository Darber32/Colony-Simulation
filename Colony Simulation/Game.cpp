#include "Headers/Game.h"
#include "Headers/Main.h"
#include "Headers/People.h"
#include "Headers/Resources.h"
#include "Headers/Boar.h"
#include "Headers/Chicken.h"
#include "Headers/Messenger.h"
#include "Headers/House.h"
#include "Headers/Storage.h"
#include "Headers/Kitchen.h"
#include "Headers/Village.h"
#include <fstream>
#include <iostream>

Game * Game::instance = nullptr;

Game::Game()
{
	
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			objects_arr[i][j] = nullptr;
	Create_Map();
	Fill_Map();
}

Game::~Game()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			if (objects_arr[i][j] != nullptr)
				delete objects_arr[i][j];
	Village::Destroy_Instance();
}

Game* Game::Get_Instance()
{
	if (instance == nullptr)
		instance = new Game();
	return instance;
}

void Game::Destroy_Instance()
{

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

Object* Game::Get_Object(int i, int j)
{
	return objects_arr[i][j];
}

void Game::Set_Object(int i, int j, Object* object)
{
	objects_arr[i][j] = object;
}

void Game::Create_Map()
{
	std::ifstream f("Map.txt");
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			f >> text_arr[i][j];
	sf::Vector2u vector;
	sf::Vector2f pos;
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			switch (text_arr[i][j])
			{
			case 'g':
				texture_arr[i][j].loadFromFile("Images/Ground/Grass.png");
				vector = texture_arr[i][j].getSize();
				pos.x = j * WIN_WIDTH / MAP_WIDTH;
				pos.y = i * WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setPosition(pos);
				pos.x = WIN_WIDTH / MAP_WIDTH;
				pos.y = WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setSize(pos);
				ground_arr[i][j].setTexture(&texture_arr[i][j]);
				ground_arr[i][j].setTextureRect(sf::IntRect(vector.x / 5 * 2, vector.y, vector.x / 5, vector.y / 5));
				break;
			case 'l':
				texture_arr[i][j].loadFromFile("Images/Ground/Grass.png");
				vector = texture_arr[i][j].getSize();
				pos.x = j * WIN_WIDTH / MAP_WIDTH;
				pos.y = i * WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setPosition(pos);
				pos.x = WIN_WIDTH / MAP_WIDTH;
				pos.y = WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setSize(pos);
				ground_arr[i][j].setTexture(&texture_arr[i][j]);
				ground_arr[i][j].setTextureRect(sf::IntRect(vector.x / 5, vector.y, vector.x / 5, vector.y));
				break;
			case 'w':
				texture_arr[i][j].loadFromFile("Images/Ground/Shore.png");
				vector = texture_arr[i][j].getSize();
				pos.x = j * WIN_WIDTH / MAP_WIDTH;
				pos.y = i * WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setPosition(pos);
				pos.x = WIN_WIDTH / MAP_WIDTH;
				pos.y = WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setSize(pos);
				ground_arr[i][j].setTexture(&texture_arr[i][j]);
				ground_arr[i][j].setTextureRect(sf::IntRect(vector.x / 5 * 4, vector.y, vector.x / 5, vector.y));
				break;
			case 's':
				texture_arr[i][j].loadFromFile("Images/Ground/Shore.png");
				vector = texture_arr[i][j].getSize();
				pos.x = j * WIN_WIDTH / MAP_WIDTH;
				pos.y = i * WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setPosition(pos);
				pos.x = WIN_WIDTH / MAP_WIDTH;
				pos.y = WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setSize(pos);
				ground_arr[i][j].setTexture(&texture_arr[i][j]);
				ground_arr[i][j].setTextureRect(sf::IntRect(0, vector.y, vector.x / 5, vector.y));
				break;
			case 'h':
				texture_arr[i][j].loadFromFile("Images/Ground/Shore.png");
				vector = texture_arr[i][j].getSize();
				pos.x = j * WIN_WIDTH / MAP_WIDTH;
				pos.y = i * WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setPosition(pos);
				pos.x = WIN_WIDTH / MAP_WIDTH;
				pos.y = WIN_HEIGHT / MAP_HEIGHT;
				ground_arr[i][j].setSize(pos);
				ground_arr[i][j].setTexture(&texture_arr[i][j]);
				ground_arr[i][j].setTextureRect(sf::IntRect(vector.x / 5, vector.y, vector.x / 5, vector.y));
				break;
			}
}

void Game::Create_Object(std::string type)
{
	sf::Vector2f pos;
	int x, y;
	do
	{
		x = rand() % (MAP_WIDTH - 6) + 3;
		y = rand() % (MAP_HEIGHT - 6) + 3;
	} while (objects_arr[y][x] != nullptr);
	pos.x = x * WIN_WIDTH / MAP_WIDTH;
	pos.y = y * WIN_HEIGHT / MAP_HEIGHT;
	if (type == "woodcutter" or type == "miner" or type == "hunter" or type == "builder")
		objects_arr[y][x] = new People(pos, type);
	else if (type == "tree" or type == "rock")
		objects_arr[y][x] = new Resources(pos, type);
	else if (type == "boar")
		objects_arr[y][x] = new Boar(pos);
	else if (type == "chicken")
		objects_arr[y][x] = new Chicken(pos);
}

void Game::Create_New_Object()
{
	static unsigned short int c = 0;
	c++;
	if (c == 10)
	{
		c = 0;
		int r = rand() % 4;
		switch (r)
		{
		case 0:
			Create_Object("tree");
			break;
		case 1:
			Create_Object("rock");
			break;
		case 2:
			Create_Object("boar");
			break;
		case 3:
			Create_Object("chicken");
			break;
		}
	}
}

void Game::Fill_Map()
{
	sf::Vector2f pos;
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			Create_Object("woodcutter");
			break;
		case 1:
			Create_Object("miner");
			break;
		case 2:
			Create_Object("hunter");
			break;
		case 3:
			Create_Object("builder");
			break;
		}
		Village::Get_Instance()->Change_Count(1, "people");
	}
	int count = rand() % 6 + 5;
	for (int i = 0; i < count; i++)
		Create_Object("tree");

	count = rand() % 6 + 5;
	for (int i = 0; i < count; i++)
		Create_Object("rock");
	count = rand() % 3 + 3;
	for (int i = 0; i < count; i++)
		Create_Object("boar");
	count = rand() % 3 + 3;
	for (int i = 0; i < count; i++)
		Create_Object("chicken");
}

void Game::Draw()
{
	window.clear();
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			window.draw(ground_arr[i][j]);
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			if (objects_arr[i][j] != nullptr)
				objects_arr[i][j]->Draw();
}

void Game::Start()
{
	
	sf::Clock clock;
	float needed_time = 500, cur_time = 0;
	while (window.isOpen())
	{
		float dt = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		cur_time += dt;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.scancode)
				{
				case sf::Keyboard::Scan::Down:
					if (needed_time < 2000)
						needed_time += 100;
					break;
				case sf::Keyboard::Scan::Up:
					if (needed_time > 100)
						needed_time -= 100;
					break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (event.key.scancode)
				{
				case sf::Keyboard::Scan::Escape:
					Pause();
					clock.restart();
					break;
				}
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					int i = event.mouseButton.y / (WIN_HEIGHT / MAP_HEIGHT), j = event.mouseButton.x / (WIN_WIDTH / MAP_WIDTH);
					if (objects_arr[i][j] != nullptr)
					{
						objects_arr[i][j]->Show();
						clock.restart();
					}
				}
			}
		}
		if (cur_time >= needed_time)
		{
			
			cur_time -= needed_time;
			Update();
		}
		Draw();
		window.display();
	}
}

void Game::Update()
{
	Village::Get_Instance()->Show_Stat();
	Village::Get_Instance()->New_People();
	Create_New_Object();
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			if (objects_arr[i][j] != nullptr)
				objects_arr[i][j]->Update();
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			if (objects_arr[i][j] != nullptr)
				objects_arr[i][j]->Reset_Update();
	Messenger* m;
	while(not messages.empty())
	{
		m = messages.front();
		messages.pop_front();
		switch (m->type)
		{
		case Types::death:
			for (int i = 0; i < MAP_HEIGHT; i++)
				for (int j = 0; j < MAP_WIDTH; j++)
					if (objects_arr[i][j] == m->death.dying)
					{
						delete objects_arr[i][j];
						objects_arr[i][j] = nullptr;
					}
			break;
		case Types::empty_slot:
			if (m->empty_slot.slot != nullptr)
			{
				m->empty_slot.people->Set_Inventory(m->empty_slot.index, nullptr);
				delete m->empty_slot.slot;
			}
			break;
		case Types::move:
			if (m->move.target != nullptr)
			{
				m->move.target->Set_Target_Position(m->move.pos);
				m->move.target->Find_Shortest_Way();
			}
			break;
		case Types::attack_animal:
			if (m->attack_animal.target != nullptr)
			{
				m->attack_animal.target->Take_Damage(m->attack_animal.damage);
			}
			break;
		case Types::attack_people:
			if (m->attack_people.target != nullptr)
				m->attack_people.target->Take_Damage(m->attack_people.damage);
			break;
		}
		delete m;
	}
}

void Game::Pause()
{
	bool flag = true;
	sf::Font font;
	font.loadFromFile("consolai.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(80);
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
			case sf::Event::KeyReleased:
				switch (event.key.scancode)
				{
				case sf::Keyboard::Scan::Escape:
					flag = false;
					break;
				}
				break;
			}
		}
		if (flag)
		{
			Draw();
			text.setPosition(sf::Vector2f(WIN_WIDTH / 2 - text.getCharacterSize() / 2 * 2.5, WIN_HEIGHT / 2 - text.getCharacterSize()));
			text.setString("Pause");
			window.draw(text);
			window.display();
		}
	}
}

void Game::Send_Message(Messenger* m)
{
	messages.push_back(m);
}



