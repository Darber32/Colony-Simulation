#include "Headers/Boar.h"
#include "Headers/Main.h"
#include "Headers/Messenger.h"

Boar::Boar(sf::Vector2f pos)
{
	meat_count = rand() % 41 + 20;
	type = "boar";
	hp = 10;
	damage = 1;
	texture.loadFromFile("Images/Animals/Boar.png");
	sf::Vector2u vector = texture.getSize();
	model.setPosition(pos);
	model.setSize(sf::Vector2f(WIN_WIDTH / MAP_WIDTH, WIN_HEIGHT / MAP_HEIGHT));
	model.setTexture(&texture);
	model.setTextureRect(sf::IntRect(0, 0, vector.x / 4, vector.y / 8));
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
	Messenger* message = new Messenger;
	message->type = Types::attack_people;
	message->attack_people.target = who_target;
	message->attack_people.damage = damage;
	Game::Get_Instance()->Send_Message(message);
}

void Boar::Update()
{
	if (not was_update)
	{
		if (hp > 0)
		{
			if ( who_target != nullptr and who_target->Get_Heal_Points() <= 0)
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
			Death();
	}
}

void Boar::Interact()
{
	who_target->Attack();
	Attack();
	was_update = true;
}

void Boar::Change_Texture(int type)
{
	sf::Vector2u vector = texture.getSize();
	model.setTextureRect(sf::IntRect(0, vector.y / 8 * type, vector.x / 4, vector.y / 8));
}
