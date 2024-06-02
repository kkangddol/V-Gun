#pragma once
#include <functional>

class Stats;
using Calc = std::function<void(Stats&)>;

// ���� �ϳ��� �������
class Parts
{
public:
	Parts();

	static void Init();

	void InitParts();

public:
	bool SelectParts(uint32 index);

	bool SelectParts(Protocol::Parts parts, uint32 grade);
	bool SelectParts(pair<Protocol::Parts, uint32> parts);

	const Calc& GetCalcStats(Protocol::Parts parts, uint32 grade);
	const Calc& GetCalcStats(pair<Protocol::Parts, uint32> parts);
	
public:
	static const int32 GetPartsPrice(uint32 grade);
	static const int32 GetPartsPrice(pair<Protocol::Parts, uint32> parts);
	static Vector<int32> partsPrice;

public:
	// Todo
	// void WriteShopPartsList(Protocol::PartsList* partsList);

public:
	// Todo 
	const Vector<pair<Protocol::Parts, uint32>>& GenerateShopPartsList(uint32 rank);
	void WriteShopPartsList(Protocol::PartsList* partsList);

private:
	Vector<std::pair<Protocol::Parts, uint32>> GenRandomAllParts(const uint32 count, bool isVGunInclude = false);
	Vector<std::pair<Protocol::Parts, uint32>> GenRandomPartsGrade(const uint32 grade, const  uint32 count = 1);
	std::pair<Protocol::Parts, uint32> GenRandomPartsVGun();

public:
	// ���� �ִ� ���� ���
	Vector<pair<Protocol::Parts, uint32>> myPartsList;
	//Map<Protocol::Parts, uint32> myPartsList;

	// �������� ������� ���� ������
	void ClearShopPartsList();
	Vector<pair<Protocol::Parts, uint32>> shopPartsList;

private:
	USE_LOCK;
	// �ε��� 0���� �Ⱦ�
	Map<Protocol::Parts, Vector<bool>> myPartsTable;
	Map<Protocol::Parts, Vector<bool>> myVGunTable;

	static Map<Protocol::Parts, Map<uint32, Calc>> partsTable;
};

