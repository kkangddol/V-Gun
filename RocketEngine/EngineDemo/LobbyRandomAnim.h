#pragma once
#include "RocketAPILoader.h"

class LobbyRandomAnim : public RocketEngine::Component
{
public:
	LobbyRandomAnim(RocketEngine::GameObject* owner);

protected:
	virtual void Start() override;
};