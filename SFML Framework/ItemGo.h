#pragma once
class PlayerGo;
class ItemSpawnerGo;
class ItemGo : public GameObject
{
public:
	enum class Types
	{
		ammo,
		health,
	};

	static const int itemTypes = 2;
protected:
	Types types = Types::ammo;
	sf::Sprite body;
	std::string textureId;
	PlayerGo* player;
	ItemSpawnerGo* itemSpawner;
public:
	ItemGo(const std::string& name = "");
	~ItemGo() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const;
	sf::FloatRect GetGlobalBounds() const;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types type);
	Types GetType() { return types; }
};
