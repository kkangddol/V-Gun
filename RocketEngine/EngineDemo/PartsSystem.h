#pragma once
#include "RocketAPILoader.h"
#include "Enum.pb.h"
#include "Types.h"

class PartsSystem : public RocketEngine::Component
{
public:
	PartsSystem(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;

public:
	void Init(); // Todo partsList √ ±‚»≠ 

public:
	void AddPartsList(Protocol::Parts parts, uint32 grade);
	void AddPartsList(std::pair<Protocol::Parts, uint32>);

	void DeletePartsList(int selectIndex);

private:
	std::vector<std::pair<Protocol::Parts, uint32>> mySelectPartsList;

};

