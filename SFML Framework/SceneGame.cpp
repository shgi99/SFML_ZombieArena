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
	Scene::Enter();
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	SOUND_MGR.PlayBgm("sound/cunning_city.mp3", true);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	player->Reset();
	Utils::SetOrigin(cursor, Origins::MC);
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
	setWaveTimer = 0.f;
	score = 0;
	isGameOver = false;
	isUpgrade = false;
	isWaveReady = false;
	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	uiUpgrade->SetActive(!isUpgrade);
	uiGameOver->SetActive(isGameOver);
	// 타일맵의 경계를 가져옴
	tileMapBound = FindGo("Tile Map")->GetLocalBounds();

	// 타일맵의 경계 내에서 랜덤한 위치 설정
	sf::Vector2f pos;
	pos.x = Utils::RandomRange(tileMapBound.left, tileMapBound.left + tileMapBound.width);
	pos.y = Utils::RandomRange(tileMapBound.top, tileMapBound.top + tileMapBound.height);

	// 플레이어의 위치를 설정된 랜덤 위치로 이동
	player->SetPosition(pos);

	LoadGameData("savedata.txt");
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
	SaveGameData("savedata.txt");
	bullets.clear();
	SOUND_MGR.StopBgm();
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
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		Variables::currentLang = Languages::Korean;
		STRING_TABLE->Load();
		OnLocalize(Variables::currentLang);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		Variables::currentLang = Languages::English;
		STRING_TABLE->Load();
		OnLocalize(Variables::currentLang);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		Variables::currentLang = Languages::Japanese;
		STRING_TABLE->Load();
		OnLocalize(Variables::currentLang);
	}
	sf::Vector2f mousePos = ScreenToUi(InputMgr::GetMousePosition());
	cursor.setPosition(mousePos);

	Scene::Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num0))
	{
		SetNewWave(7);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		player->SetGunEnhanced(40);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::B))
	{
		SpawnBossZombies();
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}
	if(!isGameOver)
	{
		if (zombies.size() == 0)
		{
			if (wave == 0)
			{
				isUpgrade = false;
			}

			uiUpgrade->SetActive(isUpgrade);

			if (!isUpgrade)
			{
				setWaveTimer += dt;
				if (setWaveTimer >= 3.f)
				{
					wave++;
					SetNewWave(wave);
					SetUiHud();
					setWaveTimer = 0.f;
					isUpgrade = true;
				}
			}
		}
	}
	else
	{
		uiGameOver->SetActive(isGameOver);
		if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		{
			SCENE_MGR.ChangeScene(SceneIds::Game);
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

void SceneGame::SpawnBossZombies()
{
	ZombieGo* zombie = zombiePool.Take();
	zombies.push_back(zombie);

	ZombieGo::Types zombieType = ZombieGo::Types::Boss;
	zombie->SetType(zombieType);

	// 타일맵 경계 내 랜덤 위치 설정
	sf::Vector2f pos;
	pos.x = Utils::RandomRange(tileMapBound.left, tileMapBound.left + tileMapBound.width);
	pos.y = Utils::RandomRange(tileMapBound.top, tileMapBound.top + tileMapBound.height);

	// 좀비 위치 설정
	zombie->SetPosition(pos);
	AddGo(zombie);
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

void SceneGame::OnKillZombie(ZombieGo* zombie)
{
	score += (wave * 100);
	zombies.remove(zombie);
	SetUiHud();
}

void SceneGame::OnZombieDie(ZombieGo* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
}

void SceneGame::OnPlayerDie()
{
	isGameOver = true;
	SaveGameData("savedata.txt");
	player->OnDie();
}

void SceneGame::OnUpgrade(Upgrade up)
{
	switch (up)
	{
	case Upgrade::RateOfFire:
	{
		player->UpgradeRateOfFire();
		upgradeFireCnt++;
		break;
	}
	case Upgrade::ClipSize:
	{
		player->UpgradeClipSize();
		upgradeClipCnt++;
		break;
	}
	case Upgrade::MaxHealth:
	{
		player->UpgradeMaxhealth();
		upgradeMaxHpCnt++;
		break;
	}
	case Upgrade::RunSpeed:
	{
		player->UpgradeRunSpeed();
		upgradeRunCnt++;
		break;
	}
	case Upgrade::HealthPickups:
	{
		player->UpgradeHealthPickup();
		upgradeHealItemCnt++;
		break;
	}
	case Upgrade::AmmoPickups:
	{
		player->UpgradeAmmoPickup();
		upgradeAmmoItenCnt++;
		break;
	}
	case Upgrade::OneMoreBullet:
	{
		player->UpgradeGun();
		upgradeBullets++;
		break;
	}
	}
	SOUND_MGR.PlaySfx("sound/powerup.wav");
	isUpgrade = false;
	uiUpgrade->SetActive(isUpgrade);
}

void SceneGame::SetNewWave(int wave)
{
	if (wave == 7)
	{
		SpawnBossZombies();
	}
	else
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
}

void SceneGame::SetUiHud()
{

	uiHud->SetAmmo(player->GetCurrentAmmo(), player->GetMaxAmmo());
	uiHud->SetWave(wave);
	uiHud->SetHp(player->GetHp(), player->GetMaxHp());
	uiHud->SetScore(score);
	if (score > highScore)
	{
		highScore = score;
		uiHud->SetHighScore(score);
	}
	else
	{
		uiHud->SetHighScore(highScore);
	}
	uiHud->SetZombieCount(zombies.size());
}

void SceneGame::SaveGameData(const std::string& filename)
{
	std::ofstream outFile(filename);

	if (!outFile) {
		std::cerr << "파일을 열 수 없습니다." << std::endl;
		return;
	}
	outFile << highScore << std::endl;
	outFile << wave << std::endl;
	outFile << upgradeFireCnt << std::endl;
	outFile << upgradeClipCnt << std::endl;
	outFile << upgradeMaxHpCnt << std::endl;
	outFile << upgradeRunCnt << std::endl;
	outFile << upgradeHealItemCnt << std::endl;
	outFile << upgradeAmmoItenCnt << std::endl;
	outFile << upgradeBullets << std::endl;
	outFile.close();

	return;
}

void SceneGame::LoadGameData(const std::string& filename)
{
	std::ifstream inFile(filename);

	if (!inFile) {
		std::cerr << "파일을 열 수 없습니다." << std::endl;
		return;
	}

	inFile >> highScore;
	inFile >> wave;
	inFile >> upgradeFireCnt;
	inFile >> upgradeClipCnt;
	inFile >> upgradeMaxHpCnt;
	inFile >> upgradeRunCnt;
	inFile >> upgradeHealItemCnt;
	inFile >> upgradeAmmoItenCnt;
	inFile >> upgradeBullets;

	SetUiHud();
	SetNewWave(wave);

	for (int i = 0; i < upgradeFireCnt; i++)
	{
		player->UpgradeRateOfFire();
	}
	for (int i = 0; i < upgradeClipCnt; i++)
	{
		player->UpgradeClipSize();
	}
	for (int i = 0; i < upgradeMaxHpCnt; i++)
	{
		player->UpgradeMaxhealth();
	}
	for (int i = 0; i < upgradeRunCnt; i++)
	{
		player->UpgradeRunSpeed();
	}
	for (int i = 0; i < upgradeHealItemCnt; i++)
	{
		player->UpgradeHealthPickup();
	}
	for (int i = 0; i < upgradeAmmoItenCnt; i++)
	{
		player->UpgradeAmmoPickup();
	}
	for (int i = 0; i < upgradeBullets; i++)
	{
		player->UpgradeGun();
	}

	inFile.close();
	return;
}

