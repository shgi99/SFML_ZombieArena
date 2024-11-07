#include "stdafx.h"
#include "ZombieGo.h"
#include "PlayerGo.h"
#include "TileMap.h"
#include "SceneGame.h"
ZombieGo::ZombieGo(const std::string& name)
	: GameObject(name)
{
}

void ZombieGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void ZombieGo::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void ZombieGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void ZombieGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ZombieGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ZombieGo::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(types);
}

void ZombieGo::Release()
{
}

void ZombieGo::Reset()
{
	player = dynamic_cast<PlayerGo*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(textureId));
	damage = 20;
	isDie = false;
	eraseTimer = 0.f;
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
}

void ZombieGo::Update(float dt)
{
	attackTimer += dt;
	if (isDie)
	{
		eraseTimer += dt;
		if (eraseTimer >= 3.0f) // 3초 후 삭제
		{
			sceneGame->OnZombieDie(this); // 좀비 삭제
		}
	}

	if (player != nullptr && Utils::Distance(position, player->GetPosition()) > 20.f && !isDie)
	{
		direction = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
	}
	
	TileMap* tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Tile Map"));
	if (tileMap != nullptr)
	{
		sf::FloatRect movableBound = tileMap->GetLocalBounds();
		sf::Vector2f cellSize = tileMap->GetCellSize();
		if (position.x > movableBound.left + movableBound.width - cellSize.x)
		{
			position.x = movableBound.left + movableBound.width - cellSize.x;
		}
		if (position.x < movableBound.left + cellSize.x)
		{
			position.x = movableBound.left + cellSize.x;
		}
		if (position.y > movableBound.top + movableBound.height - cellSize.y)
		{
			position.y = movableBound.top + movableBound.height - cellSize.y;
		}
		if (position.y < movableBound.top + cellSize.y)
		{
			position.y = movableBound.top + cellSize.y;
		}
	}
	hitBox.UpdateTr(body, GetLocalBounds());
}
void ZombieGo::FixedUpdate(float dt)
{
	attackTimer += dt;

	sf::FloatRect bounds = GetGlobalBounds();
	sf::FloatRect playerBounds = player->GetGlobalBounds();

	if (bounds.intersects(playerBounds) && Utils::CheckCollision(GetHitBox(), player->GetHitBox()) && attackTimer >= attackInterval)
	{
		HitBox& boxPlayer = player->GetHitBox();
		if(Utils::CheckCollision(hitBox, boxPlayer) && attackTimer >= attackInterval)
		{
			attackTimer = 0.f;
			player->OnDamage(damage);
		}
	}
}
sf::FloatRect ZombieGo::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect ZombieGo::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}
void ZombieGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void ZombieGo::SetType(Types type)
{
	this->types = type;
	switch (this->types)
	{
	case Types::Bloater:
		textureId = "graphics/bloater.png";
		maxHp = 50;
		speed = 100.f;
		break;
	case Types::Chaser:
		textureId = "graphics/chaser.png";
		maxHp = 20;
		speed = 150.f;
		break;
	case Types::Crawler:
		textureId = "graphics/crawler.png";
		maxHp = 10;
		speed = 50.f;
		break;
	case Types::Death:
		textureId = "graphics/blood.png";
		damage = 0;
		maxHp = 0;
		speed = 0.f;
		break;
	}

	body.setTexture(TEXTURE_MGR.Get(textureId), true); // true를 넣어줘야 다른 크기여도 알맞게 생성됨
	hp = maxHp;
}


void ZombieGo::OnDamage(int d)
{
	hp -= d;
	if (hp <= 0 && sceneGame != nullptr)
	{
		SetType(Types::Death);
		isDie = true;
	}
}
