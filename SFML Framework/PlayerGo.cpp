#include "stdafx.h"
#include "PlayerGo.h"
#include "SceneGame.h"
#include "BulletGo.h"
#include "TileMap.h"
#include "ItemGo.h"

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
	body.setTexture(TEXTURE_MGR.Get(playerTexId));
	SetOrigin(originPreset);
	SetRotation(0.f);
	direction = { 1.f, 0.f };
	countAmmo = 10;
	currentAmmo = 10;
	maxAmmo = 30;
	hp = maxHp;
	shootTimer = shootDelay;
}

void PlayerGo::Update(float dt)
{
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);

	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normalize(direction);
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
	if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		shootTimer = 0.f;
		Shoot();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		Reloading();
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
	hitBox.UpdateTr(body, GetLocalBounds());
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
		currentAmmo--;
		sceneGame->SetUiHud();
	}
}

void PlayerGo::OnDamage(int d)
{
	hp -= d;
	std::cout << "Player Damaged! " << hp << std::endl;
}

void PlayerGo::ObtainItem(ItemGo* item)
{
	int itemType = (int)item->GetType();
	switch (itemType)
	{
	case 0:
		maxAmmo += 15;
		sceneGame->SetUiHud();
		break;
	case 1:
		hp += 30;
		if (hp > maxHp)
		{
			hp = maxHp;
		}
		break;
	}
}

void PlayerGo::Reloading()
{
	if (currentAmmo < countAmmo && maxAmmo > 0 && !isReloading)
	{
		isReloading = true;
		reloadTimer = 0.f;  // 재장전 시작시 타이머 리셋
	}
}

