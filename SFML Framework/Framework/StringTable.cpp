#include "stdafx.h"
#include "StringTable.h"
#include "rapidcsv.h"
std::wstring StringTable::Undefined = L"Undefined Id";
bool StringTable::Load()
{
	Release();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	switch (Variables::currentLang)
	{
	case Languages::English:
		filePath = "tables/StringTable_eng.csv";
		break;
	case Languages::Korean:
		filePath = "tables/StringTable_kor.csv";
		break;
	case Languages::Japanese:
		filePath = "tables/StringTable_jap.csv";
		break;
	}

	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		std::vector<std::string> row = doc.GetRow<std::string>(i);
		auto it = table.find(row[0]);
		if (it != table.end())
		{
			std::cout << "스트링 테이블 키 중복!" << std::endl;
			return false;
		}

		if (row.size() < 2)
		{
			std::cout << "잘못된 데이터 형" << std::endl;
			continue;
		}

		table.insert({ row[0],converter.from_bytes(row[1]) });
	}

	return true;
}

void StringTable::Release()
{
	table.clear();
}

const std::wstring& StringTable::Get(const std::string& id)
{
	return Get(id, Variables::currentLang);
}

const std::wstring& StringTable::Get(const std::string& id, Languages lang)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return (find->second);
}
