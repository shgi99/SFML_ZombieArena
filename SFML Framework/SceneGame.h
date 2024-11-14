#pragma once
#include "Scene.h"
class PlayerGo;
class ZombieGo;
class BulletGo;
class ItemSpawnerGo;
class UiHud;
class UiUpgrade;
class UiGameOver;
enum class Upgrade;
class SceneGame : public Scene
{
protected:
	PlayerGo* player = nullptr;
	ItemSpawnerGo* itemSpawner = nullptr;
	sf::FloatRect tileMapBound;

	std::list<ZombieGo*> zombies;
	ObjectPool<ZombieGo> zombiePool;

	std::list<BulletGo*> bullets;
	ObjectPool<BulletGo> bulletPool;

	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameOver;

	sf::Sprite cursor;
	bool isGameOver = false;
	bool isUpgrade = true;
	bool isWaveReady = false;
	float setWaveTimer = 0.f;
	int wave = 0;
	int score = 0;
	int highScore = 0;

	int upgradeFireCnt = 0;
	int upgradeClipCnt = 0;
	int upgradeMaxHpCnt = 0;
	int upgradeRunCnt = 0;
	int upgradeHealItemCnt = 0;
	int upgradeAmmoItenCnt = 0;
	int upgradeBullets = 0;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Draw(sf::RenderWindow& window) override;
	void Update(float dt) override;

	void SpawnZombies(int count);
	void SpawnBossZombies();

	BulletGo* TakeBullet();
	void ReturnBullet(BulletGo* bullet);

	const std::list<ZombieGo*>& GetZombieList() const { return zombies; }

	void OnKillZombie(ZombieGo* zombie);
	void OnZombieDie(ZombieGo* zombie);
	void OnPlayerDie();
	void OnUpgrade(Upgrade up);
	void SetNewWave(int wave);

	void SetUiHud();

	void SaveGameData(const std::string& filename);
	void LoadGameData(const std::string& filename);

	int GetScore() const { return score; }
	int GetHighScore() const { return highScore; }
	int GetWave() const { return wave; }
	int GetZombieCnt() const { return zombies.size(); }
};

