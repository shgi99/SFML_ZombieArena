#pragma once
class SceneGame;
class UiHud : public GameObject
{
protected:
	SceneGame* sceneGame;

	sf::Text textScore;
	sf::Text textHighScore;

	sf::Sprite iconAmmoIcon;
	sf::Text textAmmo;
	sf::RectangleShape gaugeHp;
	sf::RectangleShape gaugeMaxHp;
	sf::RectangleShape gaugeFireCoolDown;
	sf::RectangleShape gaugeMaxFireCoolDown;
	sf::RectangleShape gaugeShunpoCoolDown;
	sf::RectangleShape gaugeMaxShunpoCoolDown;

	sf::Text textWave;
	sf::Text textZombieCount;

	sf::Vector2f gaugeHpMaxSize = { 400.f, 50.f };
	sf::Vector2f gaugeFireCoolDownMaxSize = { 400.f, 20.f };
	sf::Vector2f gaugeShunpoCoolDownMaxSize = { 400.f, 20.f };
public:
	UiHud(const std::string& name = "");
	~UiHud() = default;

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

	void SetScore(int s);
	void SetHighScore(int s);
	void SetAmmo(int current, int total);
	void SetHp(int hp, int max);
	void SetFireGauge(float timer, float interval);
	void SetShunpoGauge(float timer, float interval);
	void SetWave(int w);
	void SetZombieCount(int count);

	void OnLocalize(Languages lang) override;
};
