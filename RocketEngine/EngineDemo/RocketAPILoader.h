#pragma once
#include <windows.h>
#include "..\\RocketEngine\\RocketAPI.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\BUILDTEST\\RocketEngine.lib")
//#define ENGINE_PATH L"..\\x64\\Debug\\RocketEngine.dll"
#else
#pragma comment(lib,"..\\x64\\Release\\RocketEngine.lib")
//#define ENGINE_PATH L"Engine\\RocketEngine.dll" 
#endif // _DEBUG

// class RocketAPILoader
// {
// 	// �̱���.
// public:
// 	static RocketAPILoader& Instance()
// 	{
// 		static RocketAPILoader instance;
// 		return instance;
// 	}
// 
// private:
// 	RocketAPILoader() = default;		// �̱����̱� ������ �ܺο��� ������ �� ������.
// 
// public:
// 	void Initialize();
// 
// 	CREATE_LAUNCHER CreateLauncher;
// 	RELEASE_LAUNCHER ReleaseLauncher;
// 
// 	CREATE_OBJECT CreateObject;
// 	CREATE_SCENE CreateScene;
// 	LOAD_SCENE LoadScene;
// 	GET_DELTA_TIME GetDeltaTime;
// 	GET_MAIN_CAMERA GetMainCamera;
// 	CREATE_TWEEN CreateTween;
// 
// private:
// 	HMODULE _rocketDLL;
// };
