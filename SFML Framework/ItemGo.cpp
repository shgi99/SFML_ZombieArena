#include "stdafx.h"
#include "ItemGo.h"
#include "ItemSpawnerGo.h"
#include "PlayerGo.h"
ItemGo::ItemGo(const std::string& name)
	: GameObject(name)
{
}

void ItemGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void ItemGo::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void ItemGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void ItemGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ItemGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect ItemGo::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect ItemGo::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void ItemGo::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;

	SetType(types);
}

void ItemGo::Release()
{
}

void ItemGo::Reset()
{
	player = dynamic_cast<PlayerGo*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	itemSpawner = dynamic_cast<ItemSpawnerGo*>(SCENE_MGR.GetCurrentScene()->FindGo("Item Spawner"));
	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
}

void ItemGo::Update(float dt)
{
	SetRotation(rotation + 0.1f); // 회전 효과
	hitBox.UpdateTr(body, GetLocalBounds());
}



void ItemGo::FixedUpdate(float dt)
{

	if (IsActive() && player != nullptr)  // 아이템이 활성 상태일 때만 체크
	{
		sf::FloatRect bounds = GetGlobalBounds();
		sf::FloatRect playerBounds = player->GetGlobalBounds();
		if (bounds.intersects(playerBounds))
		{
			HitBox& boxPlayer = player->GetHitBox();
			if(Utils::CheckCollision(hitBox, boxPlayer))
			{
				player->ObtainItem(this);  // 아이템 획득 로직 실행
				itemSpawner->RemoveItem(this);
			}
		}
	}
}

void ItemGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void ItemGo::SetType(Types type)
{
	this->types = type;
	switch (this->types)
	{
	case Types::ammo:
		textureId = "graphics/ammo_pickup.png";
		break;
	case Types::health:
		textureId = "graphics/health_pickup.png";
		break;
	}

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
}
