#pragma once
class ItemGo;
class TileMap;
class SceneGame;
class ItemSpawnerGo : public GameObject
{
protected:
	std::list<ItemGo*> items;
	ObjectPool<ItemGo> itemPool;
	TileMap* tileMap;
	SceneGame* sceneGame;
	float spawnInterval = 3.f;
	float spawnTimer = 0.f;
	int maxItem = 10;
public:
	ItemSpawnerGo(const std::string& name = "");
	~ItemSpawnerGo() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	void RemoveItem(ItemGo* item);
};
