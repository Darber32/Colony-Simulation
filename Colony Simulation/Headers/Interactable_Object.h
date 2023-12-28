#pragma once
#include "Object.h"
#include "People.h"
class Interactable_Object : public Object
{
protected:
	People* who_target;
	virtual void Interact() = 0;
public:
	Interactable_Object();
	void Set_Target_People(People* unit);
	Object* Get_Target_People();
};

