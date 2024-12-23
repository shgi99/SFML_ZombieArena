#pragma once
class SceneGame;
class ItemGo;
class TileMap;
class UiHud;
class PlayerGo : public GameObject
{
protected:
	sf::Sprite body;
	std::string playerTexId = "graphics/player.png";
	std::string bloodTexId = "graphics/blood.png";
	float speed = 300.f;
	int maxHp = 100;
	int hp = 0;

	int currentAmmo = 10;
	int countAmmo = 10;
	int maxAmmo = 30;
	int gunEnhanced = 0;

	int plusMaxHp = 0;
	int plusMaxAmmo = 0;

	sf::Vector2f direction;
	sf::Vector2f look;
	sf::Color color;

	SceneGame* sceneGame;
	TileMap* tileMap;
	UiHud* uiHud;

	float shootDelay = 0.5f;
	float shootTimer = 0.f;
	float reloadTime = 2.f;
	float reloadTimer = 0.f;
	float invincibleTime = 1.f;
	float invincibleTimer = 0.f;
	float shunpoCooldown = 5.0f; 
	float shunpoTimer = 0.0f;

	bool isInvincible = false;
	bool isDie = false;
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
	void ShunPo();

	void OnDamage(int d);
	void ObtainItem(ItemGo* item);
	void Reloading();
	void OnDie();
	
	void UpgradeRateOfFire() { shootDelay *= 0.9f; }
	void UpgradeClipSize() { countAmmo += 2; }
	void UpgradeMaxhealth() { maxHp += 50; }
	void UpgradeRunSpeed() { speed *= 1.1f; }
	void UpgradeHealthPickup() { plusMaxHp += 10; }
	void UpgradeAmmoPickup() { plusMaxAmmo += 2; }
	void UpgradeGun() { gunEnhanced++; }
	void SetGunEnhanced(int gunEnhance) { gunEnhanced = gunEnhance; }

	int GetCurrentAmmo() const { return currentAmmo; }
	int GetMaxAmmo() const { return maxAmmo; }
	int GetHp() const { return hp; }
	int GetMaxHp() const { return maxHp; }
};
