#include "stdafx.h"
#include "BulletGo.h"
#include "SceneGame.h"
#include "ZombieGo.h"
BulletGo::BulletGo(const std::string& name)
	: GameObject(name)
{
}

void BulletGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void BulletGo::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void BulletGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void BulletGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void BulletGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect BulletGo::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect BulletGo::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}


void BulletGo::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void BulletGo::Release()
{
}

void BulletGo::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::ML);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.5f, 1.5f });

	direction = { 0.f, 0.f };
	speed = 20;
	damage = 10.f;
}

void BulletGo::Update(float dt)
{
	SetPosition(position + direction * speed * dt);
	hitBox.UpdateTr(body, GetLocalBounds());
}

void BulletGo::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
	{
		return;
	}
	const auto& list = sceneGame->GetZombieList();
	for (auto zombie : list)
	{
		if (!zombie->IsActive())
		{
			continue;
		}
		sf::FloatRect bounds = GetGlobalBounds();
		sf::FloatRect zombieBounds = zombie->GetGlobalBounds();
		if (bounds.intersects(zombieBounds))
		{
			HitBox& boxZombie = zombie->GetHitBox();
			if (Utils::CheckCollision(hitBox, boxZombie) && zombie->GetType() != ZombieGo::Types::Death)
			{
				zombie->OnDamage(damage);
				sceneGame->ReturnBullet(this);
			}
			break;
		}
	}
}

void BulletGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void BulletGo::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float s, int d)
{
	SetPosition(pos);
	direction = dir;
	speed = s;
	damage = d;

	SetRotation(Utils::Angle(direction));
}
