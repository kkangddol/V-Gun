#include "PanelSystem.h"

PanelSystem::PanelSystem()
{
	_engine = new RocketEngine::EngineProcess;
}

PanelSystem::~PanelSystem()
{
	delete _engine;
}

void PanelSystem::Start()
{

}

void PanelSystem::Update()
{
	
}
