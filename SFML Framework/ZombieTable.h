#pragma once
#include "DataTable.h"
#include "ZombieGo.h"
struct DataZombie
{
	ZombieGo::Types id;
	std::string textureId;
	int damage = 0;
	int maxHp = 0;
	float speed = 0.f;
};

class ZombieTable : public DataTable
{
public:
	static DataZombie Undefined;
protected:
	std::unordered_map<ZombieGo::Types, DataZombie> table;
	std::string filePath = "tables/ZombieTable.csv";
public:
	ZombieTable() : DataTable(DataTable::Types::Zombie) {}
	~ZombieTable() = default;

	bool Load() override;
	void Release() override;

	const DataZombie& Get(ZombieGo::Types id);
	
};

