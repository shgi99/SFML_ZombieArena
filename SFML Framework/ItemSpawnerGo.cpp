#include "stdafx.h"
#include "ItemSpawnerGo.h"
#include "ItemGo.h"
#include "TileMap.h"
#include "SceneGame.h"
ItemSpawnerGo::ItemSpawnerGo(const std::string& name)
	: GameObject(name)
{
}

void ItemSpawnerGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void ItemSpawnerGo::SetRotation(float angle)
{
	rotation = angle;
}

void ItemSpawnerGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void ItemSpawnerGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void ItemSpawnerGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void ItemSpawnerGo::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = -1;
}

void ItemSpawnerGo::Release()
{
	for (auto item : items)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(item);
		itemPool.Return(item);
	}
	items.clear();
}

void ItemSpawnerGo::Reset()
{
	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Tile Map"));
	for (auto item : items)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(item);
		itemPool.Return(item);
	}
	items.clear();
	spawnTimer = 0.f;
}

void ItemSpawnerGo::Update(float dt)
{
	// 아이템 스폰 로직
	if (items.size() < maxItem)
	{
		spawnTimer += dt;
		if (spawnTimer >= spawnInterval)
		{
			spawnTimer = 0.f;
			ItemGo* newItem = itemPool.Take();

			if (newItem)
			{
				// 타일맵 범위 내 무작위 위치 설정
				sf::FloatRect tileMapBounds = tileMap->GetLocalBounds();
				sf::Vector2f cellSize = tileMap->GetCellSize();
				float randomX = Utils::RandomRange(tileMapBounds.left + cellSize.x, tileMapBounds.left + tileMapBounds.width - cellSize.x);
				float randomY = Utils::RandomRange(tileMapBounds.top + cellSize.y, tileMapBounds.top + tileMapBounds.height - cellSize.y);

				newItem->SetPosition({ randomX, randomY });
				newItem->SetType((ItemGo::Types)Utils::RandomRange(0, ItemGo::itemTypes - 1)); // 타입 설정
				newItem->SetActive(true);
				items.push_back(newItem);
				SCENE_MGR.GetCurrentScene()->AddGo(newItem);
			}
		}
	}

	// 활성 아이템 업데이트
	for (auto item : items)
	{
		if (item->IsActive())
		{
			item->Update(dt);
		}
	}
}


void ItemSpawnerGo::Draw(sf::RenderWindow& window)
{
	for (auto item : items)
	{
		if (item->IsActive())
		{
			item->Draw(window);
		}
	}
}

void ItemSpawnerGo::RemoveItem(ItemGo* item)
{
	SCENE_MGR.GetCurrentScene()->RemoveGo(item);
	itemPool.Return(item);
	items.remove(item);
}
