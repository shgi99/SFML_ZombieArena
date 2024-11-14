#pragma once
#include "DataTable.h"
#include "ItemGo.h"
struct DataItem
{
	ItemGo::Types id;
	std::string textureId;
};

class ItemTable : public DataTable
{
public:
	static DataItem Undefined;
protected:
	std::unordered_map<ItemGo::Types, DataItem> table;
	std::string filePath = "tables/ItemTable.csv";
public:
	ItemTable() : DataTable(DataTable::Types::Item) {}
	~ItemTable() = default;

	bool Load() override;
	void Release() override;

	const DataItem& Get(ItemGo::Types id);
};
