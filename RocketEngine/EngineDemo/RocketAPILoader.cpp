// #include "RocketAPILoader.h"
// #include <cassert>
// 
// RocketAPILoader& rocket = RocketAPILoader::Instance();
// 
// const char* CREATE_LAUNCHER_NAME = "CreateLauncher";
// const char* RELEASE_LAUNCHER_NAME = "ReleaseLauncher";
// const char* CREATE_OBJECT_NAME = "CreateObject";
// const char* CREATE_SCENE_NAME = "CreateScene";
// const char* LOAD_SCENE_NAME = "LoadScene";
// const char* GET_DELTA_TIME_NAME = "GetDeltaTime";
// const char* GET_MAIN_CAMERA_NAME = "GetMainCamera";
// const char* CREATE_TWEEN_NAME = "CreateTween";
// 
// void RocketAPILoader::Initialize()
// {
// 	_rocketDLL = LoadLibrary(ENGINE_PATH);
// 
// 	assert(_rocketDLL);
// 
// 	CreateLauncher = reinterpret_cast<CREATE_LAUNCHER>(GetProcAddress(_rocketDLL, CREATE_LAUNCHER_NAME));
// 	ReleaseLauncher = reinterpret_cast<RELEASE_LAUNCHER>(GetProcAddress(_rocketDLL, RELEASE_LAUNCHER_NAME));
// 	CreateObject = reinterpret_cast<CREATE_OBJECT>(GetProcAddress(_rocketDLL, CREATE_OBJECT_NAME));
// 	CreateScene = reinterpret_cast<CREATE_SCENE>(GetProcAddress(_rocketDLL, CREATE_SCENE_NAME));
// 	LoadScene = reinterpret_cast<LOAD_SCENE>(GetProcAddress(_rocketDLL, LOAD_SCENE_NAME));
// 	GetDeltaTime = reinterpret_cast<GET_DELTA_TIME>(GetProcAddress(_rocketDLL, GET_DELTA_TIME_NAME));
// 	GetMainCamera = reinterpret_cast<GET_MAIN_CAMERA>(GetProcAddress(_rocketDLL, GET_MAIN_CAMERA_NAME));
// 	CreateTween = reinterpret_cast<CREATE_TWEEN>(GetProcAddress(_rocketDLL, CREATE_TWEEN_NAME));
// }
