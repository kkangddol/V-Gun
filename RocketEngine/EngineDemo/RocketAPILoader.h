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
// 	// 싱글턴.
// public:
// 	static RocketAPILoader& Instance()
// 	{
// 		static RocketAPILoader instance;
// 		return instance;
// 	}
// 
// private:
// 	RocketAPILoader() = default;		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.
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
