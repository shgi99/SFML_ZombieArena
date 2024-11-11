#include "stdafx.h"
#include "UiHud.h"
#include "SceneGame.h"
UiHud::UiHud(const std::string& name)
	: GameObject(name)
{
}

void UiHud::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHud::SetRotation(float angle)
{
	rotation = angle;
}

void UiHud::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHud::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHud::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHud::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = -1;
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	float textSize = 50.f;
	sf::Font& font = FONT_MGR.Get("fonts/malgun.ttf");

	textScore.setFont(font);
	textScore.setCharacterSize(textSize);
	textScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textScore, Origins::TL);
	textHighScore.setFont(font);
	textHighScore.setCharacterSize(textSize);
	textHighScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textHighScore, Origins::TR);
	textAmmo.setFont(font);
	textAmmo.setCharacterSize(textSize);
	textAmmo.setFillColor(sf::Color::White);
	Utils::SetOrigin(textAmmo, Origins::BL);
	textWave.setFont(font);
	textWave.setCharacterSize(textSize);
	textWave.setFillColor(sf::Color::White);
	Utils::SetOrigin(textWave, Origins::BR);
	textZombieCount.setFont(font);
	textZombieCount.setCharacterSize(textSize);
	textZombieCount.setFillColor(sf::Color::White);
	Utils::SetOrigin(textZombieCount, Origins::BR);

	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpMaxSize);
	gaugeHp.setOutlineColor(sf::Color::Black);
	gaugeHp.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeHp, Origins::BL);
	gaugeMaxHp.setFillColor(sf::Color({80, 80, 80}));
	gaugeMaxHp.setSize(gaugeHpMaxSize);
	gaugeMaxHp.setOutlineColor(sf::Color::Black);
	gaugeMaxHp.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeMaxHp, Origins::BL);

	gaugeFireCoolDown.setFillColor(sf::Color::Green);
	gaugeFireCoolDown.setSize(gaugeFireCoolDownMaxSize);
	gaugeFireCoolDown.setOutlineColor(sf::Color::Black);
	gaugeFireCoolDown.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeFireCoolDown, Origins::BL);
	gaugeMaxFireCoolDown.setFillColor(sf::Color({ 80, 80, 80 }));
	gaugeMaxFireCoolDown.setSize(gaugeFireCoolDownMaxSize);
	gaugeMaxFireCoolDown.setOutlineColor(sf::Color::Black);
	gaugeMaxFireCoolDown.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeMaxFireCoolDown, Origins::BL);

	gaugeShunpoCoolDown.setFillColor(sf::Color::Blue);
	gaugeShunpoCoolDown.setSize(gaugeShunpoCoolDownMaxSize);
	gaugeShunpoCoolDown.setOutlineColor(sf::Color::Black);
	gaugeShunpoCoolDown.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeShunpoCoolDown, Origins::BL);
	gaugeMaxShunpoCoolDown.setFillColor(sf::Color({ 80, 80, 80 }));
	gaugeMaxShunpoCoolDown.setSize(gaugeShunpoCoolDownMaxSize);
	gaugeMaxShunpoCoolDown.setOutlineColor(sf::Color::Black);
	gaugeMaxShunpoCoolDown.setOutlineThickness(3.f);
	Utils::SetOrigin(gaugeMaxShunpoCoolDown, Origins::BL);


	iconAmmoIcon.setTexture(TEXTURE_MGR.Get("graphics/ammo_icon.png"));
	Utils::SetOrigin(iconAmmoIcon, Origins::BL);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	float topY = 25.f;
	float bottomY = size.y - 25.f;

	textScore.setPosition(25.f, topY);
	textHighScore.setPosition(size.x - 25.f, topY);

	iconAmmoIcon.setPosition(25.f, bottomY);
	gaugeHp.setPosition(300.f, bottomY);
	gaugeMaxHp.setPosition(300.f, bottomY);
	gaugeFireCoolDown.setPosition(300.f, bottomY - gaugeHpMaxSize.y - 10.f);
	gaugeMaxFireCoolDown.setPosition(300.f, bottomY - gaugeHpMaxSize.y  - 10.f);
	gaugeShunpoCoolDown.setPosition(300.f, bottomY - gaugeFireCoolDownMaxSize.y - gaugeHpMaxSize.y - 20.f);
	gaugeMaxShunpoCoolDown.setPosition(300.f, bottomY - gaugeFireCoolDownMaxSize.y - gaugeHpMaxSize.y - 20.f);
	textAmmo.setPosition(100.f, bottomY);
	textWave.setPosition(size.x - 400.f, bottomY);
	textZombieCount.setPosition(size.x - 25.f, bottomY);

	SetScore(0);
	SetHighScore(0);
	SetAmmo(0, 0);
	SetHp(1.f, 1.f);
	SetWave(0);
	SetZombieCount(0);
}

void UiHud::Update(float dt)
{
}

void UiHud::Draw(sf::RenderWindow& window)
{
	window.draw(textScore);
	window.draw(textHighScore);
	window.draw(iconAmmoIcon);
	window.draw(gaugeMaxHp);
	window.draw(gaugeHp);
	window.draw(gaugeMaxFireCoolDown);
	window.draw(gaugeFireCoolDown);
	window.draw(gaugeMaxShunpoCoolDown);
	window.draw(gaugeShunpoCoolDown);
	window.draw(textAmmo);
	window.draw(textWave);
	window.draw(textZombieCount);
	
}

void UiHud::SetScore(int s)
{
	textScore.setString(STRING_TABLE->Get("SCORE") + std::to_wstring(s));
	Utils::SetOrigin(textScore, Origins::TL);
}

void UiHud::SetHighScore(int s)
{
	textHighScore.setString(STRING_TABLE->Get("HIGHSCORE") + std::to_wstring(s));
	Utils::SetOrigin(textHighScore, Origins::TR);
}

void UiHud::SetAmmo(int current, int total)
{
	textAmmo.setString(std::to_string(current) + " / " + std::to_string(total));
	Utils::SetOrigin(textAmmo, Origins::BL);
}

void UiHud::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	if (value <= 0)
	{
		value = 0.f;
	}
	gaugeHp.setSize({ gaugeHpMaxSize.x * value, gaugeHpMaxSize.y });
}

void UiHud::SetFireGauge(float timer, float interval)
{
	float value = (float)timer / interval;
	if (value >= 1)
	{
		value = 1.f;
	}
	gaugeFireCoolDown.setSize({ gaugeFireCoolDownMaxSize.x * value, gaugeFireCoolDownMaxSize.y });
}

void UiHud::SetShunpoGauge(float timer, float interval)
{
	float value = (float)timer / interval;
	if (value <= 0)
	{
		value = 0.f;
	}
	gaugeShunpoCoolDown.setSize({ gaugeShunpoCoolDownMaxSize.x * value, gaugeShunpoCoolDownMaxSize.y });
}

void UiHud::SetWave(int w)
{
	textWave.setString(STRING_TABLE->Get("WAVE") + std::to_wstring(w));
	Utils::SetOrigin(textWave, Origins::BR);
}

void UiHud::SetZombieCount(int count)
{
	textZombieCount.setString(STRING_TABLE->Get("ZOMBIE_COUNT") + std::to_wstring(count));
	Utils::SetOrigin(textZombieCount, Origins::BR);
}

void UiHud::OnLocalize(Languages lang)
{
	int score = sceneGame->GetScore();
	int highScore = sceneGame->GetHighScore();
	int wave = sceneGame->GetWave();
	int zombieCnt = sceneGame->GetZombieCnt();
	textScore.setString(STRING_TABLE->Get("SCORE", lang) + std::to_wstring(score));
	textHighScore.setString(STRING_TABLE->Get("HIGHSCORE", lang) + std::to_wstring(highScore));
	textWave.setString(STRING_TABLE->Get("WAVE", lang) + std::to_wstring(wave));
	textZombieCount.setString(STRING_TABLE->Get("ZOMBIE_COUNT", lang) + std::to_wstring(zombieCnt));
}
