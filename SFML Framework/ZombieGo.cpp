#include "stdafx.h"
#include "ZombieGo.h"
#include "PlayerGo.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "ZombieTable.h"
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
	gaugeHpMaxSize = { body.getLocalBounds().width, 5.f };
	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpMaxSize);
	gaugeHp.setOutlineColor(sf::Color::Black);
	gaugeHp.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeHp, Origins::BL);
	gaugeMaxHp.setFillColor(sf::Color({ 80, 80, 80 }));
	gaugeMaxHp.setSize(gaugeHpMaxSize);
	gaugeMaxHp.setOutlineColor(sf::Color::Black);
	gaugeMaxHp.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeMaxHp, Origins::BL);
	isDie = false;
	eraseTimer = 0.f;
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
	BossattackTimer = 0.f;
	gaugeHp.setPosition(position.x, position.y - body.getLocalBounds().height );
	gaugeMaxHp.setPosition(position.x, position.y - body.getLocalBounds().height);
}

void ZombieGo::Update(float dt)
{
	if (isDie)
	{
		eraseTimer += dt;
		if (eraseTimer >= 3.0f) // 3초 후 삭제
		{
			sceneGame->OnZombieDie(this); // 좀비 삭제
		}
	}

	if (types == Types::Boss)
	{
		BossattackTimer += dt;
		if (BossattackTimer >= BossattackInterval)
		{
			speed = 2000.f;
			if (BossattackTimer >= BossattackInterval + 0.3f)
			{
				BossattackTimer = 0.f;
				speed = 50.f;
			}
		}
	}

	if (player != nullptr && Utils::Distance(position, player->GetPosition()) > 20.f && !isDie)
	{
		direction = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
	}

	gaugeHp.setPosition(position.x - body.getLocalBounds().width / 2, position.y - body.getLocalBounds().height / 2);
	gaugeMaxHp.setPosition(position.x - body.getLocalBounds().width / 2, position.y - body.getLocalBounds().height / 2);
	
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
	sf::FloatRect bounds = GetGlobalBounds();
	sf::FloatRect playerBounds = player->GetGlobalBounds();

	if (bounds.intersects(playerBounds) && Utils::CheckCollision(GetHitBox(), player->GetHitBox()))
	{
		HitBox& boxPlayer = player->GetHitBox();
		if(Utils::CheckCollision(hitBox, boxPlayer))
		{
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
	window.draw(gaugeMaxHp);
	window.draw(gaugeHp);
	hitBox.Draw(window);
}

void ZombieGo::SetType(Types type)
{
	const auto& data = ZOMBIE_TABLE->Get(type);
	types = type;
	textureId = data.textureId;
	damage = data.damage;
	maxHp = data.maxHp;
	speed = data.speed;

	body.setTexture(TEXTURE_MGR.Get(textureId), true); // true를 넣어줘야 다른 크기여도 알맞게 생성됨
	hp = maxHp;
}


void ZombieGo::OnDamage(int d)
{
	hp -= d;
	float value = (float)hp / maxHp;
	if (value <= 0)
	{
		value = 0.f;
	}
	gaugeHp.setSize({ gaugeHpMaxSize.x * value, gaugeHpMaxSize.y });
	if (hp <= 0 && sceneGame != nullptr)
	{
		SetType(Types::Death);
		SOUND_MGR.PlaySfx("sound/splat.wav");
		isDie = true;
		sceneGame->OnKillZombie(this);
		sceneGame->SetUiHud();
	}
}
