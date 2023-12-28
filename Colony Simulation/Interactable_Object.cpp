#include "Headers/Interactable_Object.h"

Interactable_Object::Interactable_Object()
{
	who_target = nullptr;
}

void Interactable_Object::Set_Target_People(People* unit)
{
	who_target = unit;
}

Object* Interactable_Object::Get_Target_People()
{
	return who_target;
}
