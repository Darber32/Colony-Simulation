#include "Headers/Kitchen.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"

Kitchen::Kitchen(sf::Vector2f pos)
{
	Village::Get_Instance()->Change_Count(1, "kitchen");
	type = "kitchen";
	cost = std::pair<int, int>(40, 20);
	Village::Get_Instance()->Remove_Materials(cost);
	texture.loadFromFile("Images/Buildings/Wood/Market.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	text_pos.left = vector.x / 3 * (rand() % 3);
	text_pos.top = vector.y / 4 * (rand() % 4);
	text_pos.width = vector.x / 3;
	text_pos.height = vector.y / 4;
	model.setTextureRect(sf::IntRect(vector.x / 3 * (rand() % 3), vector.y / 4 * (rand() % 4), vector.x / 3, vector.y / 4));
}

void Kitchen::Show()
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

void Kitchen::Interact()
{
	if (not is_anybody_inside)
	{
		Enter();
	}
	was_update = true;
	Inventory_Object* meat = nullptr;
	int index;
	bool flag = true;
	int c = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (who_target->Get_Inventory(i) != nullptr)
		{ 
			c++;
			if (who_target->Get_Inventory(i)->Get_Type() == "raw meat")
			{
				meat = who_target->Get_Inventory(i);
				index = i;
			}
		}
	}
	if (c == INVENTORY_SIZE or meat == nullptr or meat->Get_Count() == 0)
	{
		flag = false;
		Exit();
	}
	if (flag and meat != nullptr)
	{
		int count = meat->Get_Count();
		if (count > 5)
			count = 5;
		for (int i = 0; i < INVENTORY_SIZE; i++)
			if (who_target->Get_Inventory(i) and who_target->Get_Inventory(i)->Get_Type() == "cooked meat")
			{
				Village::Get_Instance()->Change_Count(-count, "raw meat");
				meat->Change_Count(-count);
				Village::Get_Instance()->Change_Count(count, "cooked meat");
				count = who_target->Get_Inventory(i)->Change_Count(count);
				meat->Change_Count(count);
				if (count == 0)
					break;
			}
		if (count > 0)
			for (int i = 0; i < INVENTORY_SIZE; i++)
				if (who_target->Get_Inventory(i) == nullptr)
				{
					Inventory_Object* cooked_meat = new Inventory_Object("cooked meat", count);
					Village::Get_Instance()->Change_Count(-count, "raw meat");
					meat->Change_Count(-count);
					Village::Get_Instance()->Change_Count(count, "cooked meat");
					who_target->Set_Inventory(i, cooked_meat);
					break;
				}
	}
	if (meat != nullptr and meat->Get_Count() == 0)
	{
		Messenger* message = new Messenger;
		message->type = Types::empty_slot;
		message->empty_slot.index = index;
		message->empty_slot.people = who_target;
		message->empty_slot.slot = meat;
		Game::Get_Instance()->Send_Message(message);
	}
}
