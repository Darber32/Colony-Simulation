#pragma once
#define MAP_WIDTH 30
#define MAP_HEIGHT 30
#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>



class Object;
struct Messenger;

class Game
{
private:
	std::list<Messenger*> messages;
	sf::Event event;
	Object* objects_arr[MAP_WIDTH][MAP_HEIGHT];
	sf::RectangleShape ground_arr[MAP_WIDTH][MAP_HEIGHT];
	sf::Texture texture_arr[MAP_WIDTH][MAP_HEIGHT];
	char text_arr[MAP_WIDTH][MAP_HEIGHT];
	static Game* instance;
	Game();
	~Game();
	void Create_Map();
	void Create_New_Object();
	void Fill_Map();
	void Draw();
	void Update();
public:
	void Create_Object(std::string type);
	static Game* Get_Instance();
	static void Destroy_Instance();
	Object* Get_Object(int i, int j);
	void Set_Object(int i, int j, Object * object);
	void Start();
	void Send_Message(Messenger* m);
};

