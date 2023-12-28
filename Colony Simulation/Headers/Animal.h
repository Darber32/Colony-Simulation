#pragma once
#include "Interactable_Object.h"
class Animal :public Interactable_Object
{
protected:
	int hp, meat_count;
	virtual void Move() = 0;
	virtual void Change_Texture(int type) = 0;
	void Death();
public:
	void Take_Damage(int damage);
};

