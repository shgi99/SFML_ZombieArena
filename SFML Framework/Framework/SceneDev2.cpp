#include "stdafx.h"
#include "SceneDev2.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{

}

void SceneDev2::Init()
{	
	auto text = AddGo(new TextGo("fonts/malgun.ttf", "Scene Name 1 "));
	auto text2 = AddGo(new TextGo("fonts/malgun.ttf", "Scene Name 2 "));
	auto text3 = AddGo(new TextGo("fonts/malgun.ttf", "Scene Name 3 "));
	Scene::Init();

	text->sortingLayer = SortingLayers::UI;
	text->Set(25, sf::Color::White);
	text->SetString("HI");

	text2->sortingLayer = SortingLayers::UI;
	text2->SetPosition({ 0.f, 100.f });
	text2->Set(25, sf::Color::White);
	text2->SetString("BYE");

	text3->sortingLayer = SortingLayers::UI;
	text3->SetPosition({ 0.f, 200.f });
	text3->Set(25, sf::Color::White);
	text3->SetString("THANK");
}

void SceneDev2::Enter()
{
	Scene::Enter();
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		Variables::currentLang = Languages::Korean;
		OnLocalize(Variables::currentLang);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		Variables::currentLang = Languages::English;
		OnLocalize(Variables::currentLang);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		Variables::currentLang = Languages::Japanese;
		OnLocalize(Variables::currentLang);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev1);
	}
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
