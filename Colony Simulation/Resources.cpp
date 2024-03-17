#include "Headers/Resources.h"
#include "Headers/Messenger.h"
#include "Headers/Main.h"
#include "Headers/Village.h"

Resources::Resources(sf::Vector2f pos, std::string type)
{
	std::string filename;
	if (type == "tree")
		filename = "Images/Resources/Trees.png";
	else if (type == "rock")
		filename = "Images/Resources/Rocks.png";
	this->type = type;
	count_of_resources = rand() % 41 + 10;
	texture.loadFromFile(filename);
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(&texture);
	if (type == "tree")
	{
		text_pos.left = vector.x / 4 * (rand() % 3 + 1);
		text_pos.top = 0;
		text_pos.width = vector.x / 4;
		text_pos.height = vector.y;
		model.setTextureRect(text_pos);
	}
	else if (type == "rock")
	{
		text_pos.left = vector.x / 3 * (rand() % 3);
		text_pos.top = vector.y / 4 * (rand() % 4);
		text_pos.width = vector.x / 3;
		text_pos.height = vector.y / 4;
		model.setTextureRect(text_pos);
	}
	interact_count = 0;
	interact_max = 5;
}

Resources::~Resources()
{
}

void Resources::Interact()
{
	who_target->Was_Update();
	interact_count++;
	if (interact_count == interact_max)
	{
		Village::Get_Instance()->Change_Count(count_of_resources, type);
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* slot = who_target->Get_Inventory(i);
			if (slot != nullptr)
			{
				if (slot->Get_Type() == type)
					count_of_resources = slot->Change_Count(count_of_resources);
				if (count_of_resources == 0)
					break;
			}
		}
		if (count_of_resources > 0)
			for (int i = 0; i < INVENTORY_SIZE; i++)
			{
				Inventory_Object* slot = who_target->Get_Inventory(i);
				if (slot == nullptr)
				{
					slot = new Inventory_Object(type, count_of_resources);
					who_target->Set_Inventory(i, slot);
					break;
				}
			}
		who_target->Reset_Target();
		Messenger* message = new Messenger;
		message->type = Types::death;
		message->death.dying = this;
		Game::Get_Instance()->Send_Message(message);
	}
}

void Resources::Update()
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
}

int Resources::Get_Resources()
{
	return count_of_resources;
}

void Resources::Show()
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

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 7, WIN_HEIGHT / 9));
			text.setString(type);
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 9 * 2));
			text.setString("Count of resources:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 7, WIN_HEIGHT / 9 * 2));
			text.setString(std::to_string(count_of_resources));
			window.draw(text);

			window.draw(image);
			window.display();
		}
	}
}
