#include "stdafx.h"
#include "PlayerGo.h"
#include "SceneGame.h"
#include "BulletGo.h"
#include "TileMap.h"
#include "ItemGo.h"
#include "UiHud.h"
PlayerGo::PlayerGo(const std::string& name)
	: GameObject(name)
{
}

void PlayerGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void PlayerGo::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void PlayerGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void PlayerGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void PlayerGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect PlayerGo::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect PlayerGo::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void PlayerGo::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
}

void PlayerGo::Release()
{
}

void PlayerGo::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = dynamic_cast<TileMap*>(sceneGame->FindGo("Tile Map"));
	uiHud = dynamic_cast<UiHud*>(sceneGame->FindGo("UI Hud"));
	body.setTexture(TEXTURE_MGR.Get(playerTexId));
	color = body.getColor();
	color.a = 255;
	body.setColor(color);
	SetOrigin(originPreset);
	SetRotation(0.f);
	isInvincible = false;
	isReloading = false;
	isDie = false;
	speed = 300.f;
	direction = { 1.f, 0.f };
	countAmmo = 10;
	currentAmmo = 10;
	maxAmmo = 30;
	maxHp = 100;
	plusMaxHp = 30;
	plusMaxAmmo = 10;
	hp = maxHp;
	shootTimer = shootDelay;
	shunpoTimer = shunpoCooldown;
	gunEnhanced = 0;
}

void PlayerGo::Update(float dt)
{
	if(!isDie)
	{
		direction.x = InputMgr::GetAxis(Axis::Horizontal);
		direction.y = InputMgr::GetAxis(Axis::Vertical);

		float mag = Utils::Magnitude(direction);
		if (mag > 1.f)
		{
			Utils::Normalize(direction);
		}
		uiHud->SetShunpoGauge(shunpoTimer, shunpoCooldown);
		if (shunpoTimer < shunpoCooldown)
		{
			shunpoTimer += dt;
		}

		sf::Vector2i mousePos = InputMgr::GetMousePosition();
		sf::Vector2f mouseWorldPos = (sf::Vector2f)mousePos;
		if (sceneGame != nullptr)
		{
			mouseWorldPos = sceneGame->ScreenToWorld(mousePos);
		}
		look = Utils::GetNormal(mouseWorldPos - position);

		SetRotation(Utils::Angle(look));
		SetPosition(position + direction * speed * dt);

		shootTimer += dt;
		uiHud->SetFireGauge(shootTimer, shootDelay);
		if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			Shoot();
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::R))
		{
			Reloading();
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Space) && shunpoTimer >= shunpoCooldown)
		{
			ShunPo();
		}
		if (isReloading)
		{
			reloadTimer += dt;
			if (reloadTimer >= reloadTime)  // 재장전이 완료되면
			{
				int ammoNeeded = countAmmo - currentAmmo;   // 한 탄창에 채워야 할 탄약 수
				int ammoToReload = std::min(ammoNeeded, maxAmmo);  // 소지 탄약에서 채울 수 있는 만큼
				currentAmmo += ammoToReload;
				maxAmmo -= ammoToReload;
				isReloading = false;
				sceneGame->SetUiHud();  // UI 업데이트
			}
		}
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
		if (isInvincible)
		{
			invincibleTimer += dt;
			if (invincibleTimer >= invincibleTime)
			{
				color.a = 255;
				body.setColor(color);
				isInvincible = false;
				invincibleTimer = 0.f;
			}
		}
		hitBox.UpdateTr(body, GetLocalBounds());
	}
}

void PlayerGo::FixedUpdate(float dt)
{
}

void PlayerGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void PlayerGo::Shoot()
{
	if(currentAmmo > 0 && !isReloading)
	{

		BulletGo* bullet = sceneGame->TakeBullet();
		bullet->Fire(position, look, 800.f, 10);
		if (gunEnhanced == 1)
		{
			BulletGo* extrabullet = sceneGame->TakeBullet();
			sf::Vector2f rotatedLook = Utils::RotateVector(look, 10.f);
			extrabullet->Fire(position, rotatedLook, 800.f, 10);
		}

		for (int i = 2; i <= gunEnhanced; i++)
		{
			BulletGo* extrabullet = sceneGame->TakeBullet();
			if (i % 2 == 1)
			{
				sf::Vector2f rotatedLook = Utils::RotateVector(look, 10.f * ((i + 1) / 2.0f));
				extrabullet->Fire(position, rotatedLook, 800.f, 10);
			}
			else
			{
				sf::Vector2f rotatedLook = Utils::RotateVector(look, -10.f * ((i + 1) / 2.0f));
				extrabullet->Fire(position, rotatedLook, 800.f, 10);
			}
		}
		currentAmmo--;
		shootTimer = 0.f;
		SOUND_MGR.PlaySfx("sound/shoot.wav");
		sceneGame->SetUiHud();
	}
}

void PlayerGo::ShunPo()
{
	const float shunpoDistance = 200.0f; 

	if (Utils::Magnitude(direction) > 0.f) 
	{
		sf::Vector2f targetPosition = position + direction * shunpoDistance;

		if (tileMap != nullptr)
		{
			sf::FloatRect movableBound = tileMap->GetLocalBounds();
			sf::Vector2f cellSize = tileMap->GetCellSize();

			if (targetPosition.x > movableBound.left + movableBound.width - cellSize.x)
			{
				targetPosition.x = movableBound.left + movableBound.width - cellSize.x;
			}
			if (targetPosition.x < movableBound.left + cellSize.x)
			{
				targetPosition.x = movableBound.left + cellSize.x;
			}
			if (targetPosition.y > movableBound.top + movableBound.height - cellSize.y)
			{
				targetPosition.y = movableBound.top + movableBound.height - cellSize.y;
			}
			if (targetPosition.y < movableBound.top + cellSize.y)
			{
				targetPosition.y = movableBound.top + cellSize.y;
			}
		}

		SetPosition(targetPosition);
		shunpoTimer = 0.0f;

		isInvincible = true;
		invincibleTimer = 0.f;  
		color.a = 120;         
		body.setColor(color);
	}
}


void PlayerGo::OnDamage(int d)
{
	if(!isInvincible)
	{
		hp -= d;
		sceneGame->SetUiHud();
		isInvincible = true;
		color.a = 120;
		body.setColor(color);
		if (hp <= 0 && sceneGame != nullptr)
		{
			sceneGame->OnPlayerDie();
		}
	}
}

void PlayerGo::ObtainItem(ItemGo* item)
{
	int itemType = (int)item->GetType();
	switch (itemType)
	{
	case 0:
		maxAmmo += plusMaxAmmo;
		sceneGame->SetUiHud();
		break;
	case 1:
		hp += plusMaxHp;
		if (hp > maxHp)
		{
			hp = maxHp;
		}
		sceneGame->SetUiHud();
		break;
	}
	SOUND_MGR.PlaySfx("sound/pickup.wav");
}

void PlayerGo::Reloading()
{
	if (currentAmmo < countAmmo && maxAmmo > 0 && !isReloading)
	{
		SOUND_MGR.PlaySfx("sound/reload.wav");
		isReloading = true;
		reloadTimer = 0.f;  // 재장전 시작시 타이머 리셋
	}
	else
	{
		SOUND_MGR.PlaySfx("sound/reload_failed.wav");
	}
}

void PlayerGo::OnDie()
{
	isDie = true;
	body.setTexture(TEXTURE_MGR.Get(bloodTexId));
	speed = 0.f;
	currentAmmo = 0;
	maxAmmo = 0;
}

