#include "stdafx.h"
#include "UiUpgrade.h"
#include "SceneGame.h"
UiUpgrade::UiUpgrade(const std::string& name)
	: GameObject(name)
{
}

void UiUpgrade::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiUpgrade::SetRotation(float angle)
{
	rotation = angle;
}

void UiUpgrade::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiUpgrade::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiUpgrade::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiUpgrade::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
}

void UiUpgrade::Release()
{
}

void UiUpgrade::Reset()
{
	backGroundSprite.setTexture(TEXTURE_MGR.Get("graphics/background.png"));
	Utils::SetOrigin(backGroundSprite, Origins::TL);
	backGroundSprite.setPosition(0.f, 0.f);

	float textSize = 100.f;
	sf::Font& font = FONT_MGR.Get("fonts/malgun.ttf");

	for (int i = 0; i < _countof(text); i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(textSize);
		text[i].setFillColor(sf::Color::White);
		Utils::SetOrigin(text[i], Origins::TL);
		text[i].setPosition(100.f, 100.f + (100.f * i));
	}
	text[0].setString(STRING_TABLE->Get("UPGRADE_FIRE"));
	text[1].setString(STRING_TABLE->Get("UPGRADE_RELOAD"));
	text[2].setString(STRING_TABLE->Get("UPGRADE_MAXHP"));
	text[3].setString(STRING_TABLE->Get("UPGRADE_RUN"));
	text[4].setString(STRING_TABLE->Get("UPGRADE_HP_PICKUP"));
	text[5].setString(STRING_TABLE->Get("UPGRADE_AMMO_PICKUP"));
	text[6].setString(STRING_TABLE->Get("UPGRADE_GUN"));
}

void UiUpgrade::Update(float dt)
{
}

void UiUpgrade::FixedUpdate(float dt)
{
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (sceneGame != nullptr)
	{
		sf::Vector2f mousePos = sceneGame->ScreenToUi(InputMgr::GetMousePosition());
		for (int i = 0; i < _countof(text); i++)
		{
			if (text[i].getGlobalBounds().contains(mousePos))
			{
				text[i].setFillColor(sf::Color::Red);
				if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
				{
					sceneGame->OnUpgrade((Upgrade)i);
				}
			}
			else
			{
				text[i].setFillColor(sf::Color::White);
			}
		}
	}
}

void UiUpgrade::Draw(sf::RenderWindow& window)
{
	window.draw(backGroundSprite);
	for (int i = 0; i < _countof(text); i++)
	{
		window.draw(text[i]);
	}
}

void UiUpgrade::OnLocalize(Languages lang)
{
	text[0].setString(STRING_TABLE->Get("UPGRADE_FIRE", lang));
	text[1].setString(STRING_TABLE->Get("UPGRADE_RELOAD", lang));
	text[2].setString(STRING_TABLE->Get("UPGRADE_MAXHP", lang));
	text[3].setString(STRING_TABLE->Get("UPGRADE_RUN", lang));
	text[4].setString(STRING_TABLE->Get("UPGRADE_HP_PICKUP", lang));
	text[5].setString(STRING_TABLE->Get("UPGRADE_AMMO_PICKUP", lang));
	text[6].setString(STRING_TABLE->Get("UPGRADE_GUN", lang));
	SetOrigin(originPreset);
}
