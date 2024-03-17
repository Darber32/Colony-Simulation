#include "Headers/Storage.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"
#include "Headers/Village.h"

Storage::Storage(sf::Vector2f pos)
{
	Village::Get_Instance()->Change_Count(1, "storage");
	type = "storage";
	cur_time = 0;
	max_time = 5;
	cost = std::pair<int, int>(20, 40);
	Village::Get_Instance()->Remove_Materials(cost);
	is_full = false;
	texture.loadFromFile("Images/Buildings/Wood/Barracks.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f((WIN_WIDTH / MAP_WIDTH) * 2, (WIN_HEIGHT / MAP_HEIGHT) * 2));
	model.setTexture(&texture);
	text_pos.left = vector.x / 4 * (rand() % 4);
	text_pos.top = vector.y / 5 * (rand() % 2);
	text_pos.width = vector.x / 4;
	text_pos.height = vector.y / 5;
	model.setTextureRect(text_pos);
	for (int i = 0; i < STORAGE_SIZE; i++)
		storage[i] = nullptr;
	Village::Get_Instance()->Add_Storage(this);
}

Storage::~Storage()
{
	Village::Get_Instance()->Delete_Storage(this);
	for (int i = 0; i < STORAGE_SIZE; i++)
		if (storage[i] != nullptr)
		{
			Village::Get_Instance()->Change_Count(-storage[i]->Get_Count(), storage[i]->Get_Type());
			delete storage[i];
		}
}

Inventory_Object* Storage::Get_Storage(int index)
{
	if (index >= STORAGE_SIZE)
		return nullptr;
	return storage[index];
}

void Storage::Reset_Storage(int index)
{
	if (index < STORAGE_SIZE and storage[index] != nullptr)
	{
		storage[index] = nullptr;
		is_full = false;
	}
}

void Storage::Show()
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

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15));
			text.setString("Type:");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 6, WIN_HEIGHT / 15));
			text.setString(type);
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15 * 2));
			text.setString("Anybody in building?");
			window.draw(text);

			text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 6, WIN_HEIGHT / 15 * 2));
			if (is_anybody_inside)
				text.setString("yes");
			else
				text.setString("no");
			window.draw(text);

			for (int i = 0; i < STORAGE_SIZE; i++)
			{
				text.setPosition(sf::Vector2f(WIN_WIDTH / 9, WIN_HEIGHT / 15 * (3 + i)));
				text.setString("Slot number " + std::to_string(i + 1) + ":");
				window.draw(text);

				text.setPosition(sf::Vector2f(WIN_WIDTH / 9 * 5, WIN_HEIGHT / 15 * (3 + i)));
				if (storage[i] != nullptr)
					text.setString(storage[i]->Get_Type() + " count: " + std::to_string(storage[i]->Get_Count()));
				else
					text.setString("empty");
				window.draw(text);
			}

		

			window.draw(image);
			window.display();
		}
	}
}

bool Storage::Is_Full()
{
	return is_full;
}

void Storage::Interact()
{
	if (not is_anybody_inside)
	{
		Enter();
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* target_slot = who_target->Get_Inventory(i);
			if (target_slot != nullptr)
			{
				for (int j = 0; j < STORAGE_SIZE; j++)
				{
					if (storage[j] != nullptr and target_slot->Get_Type() == storage[j]->Get_Type())
					{
						int storage_count = storage[j]->Get_Count(), people_count = target_slot->Get_Count(), storage_avalible = storage[j]->Get_Max_Count() - storage_count;
						if (people_count <= storage_avalible)
						{
							storage[j]->Change_Count(people_count);
							target_slot->Change_Count(-people_count);
							Messenger* message = new Messenger;
							message->type = Types::empty_slot;
							message->empty_slot.index = i;
							message->empty_slot.people = who_target;
							message->empty_slot.slot = target_slot;
							Game::Get_Instance()->Send_Message(message);
						}
						else
						{
							storage[j]->Change_Count(storage_avalible);
							target_slot->Change_Count(-storage_avalible);
						}
					}
				}
			}
		}
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			Inventory_Object* target_slot = who_target->Get_Inventory(i);
			if (target_slot != nullptr and target_slot->Get_Count() != 0)
			{
				for (int j = 0; j < STORAGE_SIZE; j++)
					if (storage[j] == nullptr)
					{
						storage[j] = target_slot;
						who_target->Set_Inventory(i, nullptr);
						break;
					}
			}
		}
		int c = 0;
		for (int i = 0; i < STORAGE_SIZE; i++)
			if (storage[i] != nullptr)
				c++;
		if (c == STORAGE_SIZE)
			is_full = true;
		else
			is_full = false;
	}
	was_update = true;
	cur_time++;
	if (cur_time >= max_time)
	{
		cur_time = 0;
		Exit();
	}
}

