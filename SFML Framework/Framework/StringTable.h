#pragma once
#include "DataTable.h"
class StringTable : public DataTable
{
public:
	static std::wstring Undefined;
protected:
	// <T1, T2>
	// T1 : ��Ʈ�� ���̵�
	// T2 : �� ��Ʈ��([0] = �ѱ���, [1] = ���� ....)
	std::unordered_map<std::string, std::wstring> table;
	std::string filePath = "tables/StringTable_eng.csv";
public:
	StringTable() : DataTable(DataTable::Types::String) {}
	~StringTable() = default;

	bool Load() override;
	void Release() override;

	const std::wstring& Get(const std::string& id);
	const std::wstring& Get(const std::string& id, Languages lang);
private:
	std::wstring string_to_wstring(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());  // ������ ��ȯ
	}
};

