#pragma once
class SceneGame;
class ItemGo;
class TileMap;
class PlayerGo : public GameObject
{
protected:
	sf::Sprite body;
	std::string playerTexId = "graphics/player.png";

	float speed = 500.f;
	int maxHp = 100;
	int hp = 0;

	int currentAmmo = 10;
	int countAmmo = 10;
	int maxAmmo = 30;
	sf::Vector2f direction;
	sf::Vector2f look;

	SceneGame* sceneGame;
	TileMap* tileMap;
	float shootDelay = 0.5f;
	float shootTimer = 0.f;
	float reloadTime = 2.f;
	float reloadTimer = 0.f;

	bool isReloading = false;
public:
	PlayerGo(const std::string& name = "");
	~PlayerGo() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const;
	sf::FloatRect GetGlobalBounds() const;
	sf::Sprite GetSprite() const { return body; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Shoot();
	void OnDamage(int d);
	void ObtainItem(ItemGo* item);
	void Reloading();

	int GetCurrentAmmo() const { return currentAmmo; }
	int GetMaxAmmo() const { return maxAmmo; };
	int GetHp() const { return hp; };
	int GetMaxHp() const { return maxHp; };
};
