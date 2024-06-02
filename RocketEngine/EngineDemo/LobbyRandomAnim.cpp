#include "LobbyRandomAnim.h"

LobbyRandomAnim::LobbyRandomAnim(RocketEngine::GameObject* owner)
	:Component(owner)
{

}

void LobbyRandomAnim::Start()
{
	gameObject->GetComponent<RocketEngine::AnimationController>()->PlayCurrentAnim();
}
