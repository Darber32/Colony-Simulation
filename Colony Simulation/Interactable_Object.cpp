#include "Headers/Interactable_Object.h"

Interactable_Object::Interactable_Object()
{
	who_target = nullptr;
}

Interactable_Object::~Interactable_Object()
{
	if (who_target != nullptr and who_target->Get_Target() == this)
		who_target->Reset_Target();
}

void Interactable_Object::Set_Target_People(People* unit)
{
	who_target = unit;
}

Object* Interactable_Object::Get_Target_People()
{
	return who_target;
}
