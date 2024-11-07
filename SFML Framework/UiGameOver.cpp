#include "stdafx.h"
#include "UiGameOver.h"

UiGameOver::UiGameOver(const std::string& name)
	: GameObject(name)
{
}

void UiGameOver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameOver::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameOver::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiGameOver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(text, originPreset);
	}
}

void UiGameOver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	text.setOrigin(origin);
}

void UiGameOver::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = -1;
}

void UiGameOver::Release()
{
}

void UiGameOver::Reset()
{
	text.setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
	text.setCharacterSize(150.f);
	text.setPosition(FRAMEWORK.GetWindowSizeF() * 0.5f);
	text.setFillColor(sf::Color::Red);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);
	text.setString("PRESS ENTER\nTO CONTINUE");
	Utils::SetOrigin(text, Origins::MC);
}

void UiGameOver::Update(float dt)
{
}

void UiGameOver::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}
