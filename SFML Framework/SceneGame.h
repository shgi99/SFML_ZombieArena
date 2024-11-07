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
	float setWaveTimer = 0.f;
	int wave = 0;
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

	BulletGo* TakeBullet();
	void ReturnBullet(BulletGo* bullet);

	const std::list<ZombieGo*>& GetZombieList() const { return zombies; }

	void OnZombieDie(ZombieGo* zombie);
	void OnUpgrade(Upgrade up);
	void SetNewWave(int wave);

	void SetUiHud();
};

