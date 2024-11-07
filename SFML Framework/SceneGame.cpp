#include "stdafx.h"
#include "SceneGame.h"
#include "PlayerGo.h"
#include "TileMap.h"
#include "ZombieGo.h"
#include "BulletGo.h"
#include "ItemSpawnerGo.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	AddGo(new TileMap("Tile Map"));

	player = AddGo(new PlayerGo("Player"));
	itemSpawner = AddGo(new ItemSpawnerGo("Item Spawner"));
	uiHud = AddGo(new UiHud("UI Hud"));
	uiUpgrade = AddGo(new UiUpgrade("UI Upgrade"));
	uiGameOver = AddGo(new UiGameOver("UI Game Over"));

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
	wave = 0;
	setWaveTimer = 0.f;
	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	// 타일맵의 경계를 가져옴
	tileMapBound = FindGo("Tile Map")->GetLocalBounds();

	// 타일맵의 경계 내에서 랜덤한 위치 설정
	sf::Vector2f pos;
	pos.x = Utils::RandomRange(tileMapBound.left, tileMapBound.left + tileMapBound.width);
	pos.y = Utils::RandomRange(tileMapBound.top, tileMapBound.top + tileMapBound.height);

	// 플레이어의 위치를 설정된 랜덤 위치로 이동
	player->SetPosition(pos);

	Scene::Enter();
	SetUiHud();
}

void SceneGame::Exit()
{
	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();
	Scene::Exit();
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	const sf::View& saveView = window.getView();
	window.setView(uiView);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::Update(float dt)
{
	sf::Vector2f mousePos = ScreenToUi(InputMgr::GetMousePosition());
	cursor.setPosition(mousePos);

	Scene::Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::BackSpace))
	{
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}

	//if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	//{
	//	SpawnZombies(10);
	//}

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		uiGameOver->SetActive(!uiGameOver->IsActive());
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

	if (zombies.size() == 0)
	{
		setWaveTimer += dt;
		if (setWaveTimer >= 3.f)
		{
			wave++;
			SetNewWave(wave);
			SetUiHud();
			setWaveTimer = 0.f;
		}
	}
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		ZombieGo* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		ZombieGo::Types zombieType = (ZombieGo::Types)Utils::RandomRange(0, ZombieGo::zombieTypes - 1);
		zombie->SetType(zombieType);

		// 타일맵 경계 내 랜덤 위치 설정
		sf::Vector2f pos;
		pos.x = Utils::RandomRange(tileMapBound.left, tileMapBound.left + tileMapBound.width);
		pos.y = Utils::RandomRange(tileMapBound.top, tileMapBound.top + tileMapBound.height);

		// 좀비 위치 설정
		zombie->SetPosition(pos);
		AddGo(zombie);
	}
}

BulletGo* SceneGame::TakeBullet()
{
	BulletGo* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);

	return bullet;
}

void SceneGame::ReturnBullet(BulletGo* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::OnZombieDie(ZombieGo* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
}

void SceneGame::OnUpgrade(Upgrade up)
{
	uiUpgrade->SetActive(false);
	std::cout << (int) up;
}

void SceneGame::SetNewWave(int wave)
{
	SpawnZombies(10 * wave);
	for (auto zombie : zombies)
	{
		zombie->SetMaxHp(zombie->GetMaxHp() + ((wave - 1) * 10));
		zombie->SetHp(zombie->GetMaxHp());
		zombie->SetDamage(zombie->GetDamage() + ((wave - 1) * 5));
		zombie->SetSpeed(zombie->GetSpeed() + ((wave - 1) * 10.f));
	}
}

void SceneGame::SetUiHud()
{
	uiHud->SetAmmo(player->GetCurrentAmmo(), player->GetMaxAmmo());
	uiHud->SetWave(wave);
}

