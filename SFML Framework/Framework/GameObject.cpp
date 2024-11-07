#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	: name(name), originPreset(Origins::TL)
{

}

const HitBox& GameObject::GetHitBox() const
{
	return hitBox;
}

HitBox& GameObject::GetHitBox()
{
	return hitBox;
}

void GameObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = { 0.f, 0.f };
}
