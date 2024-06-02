// ���ø����̼ǿ� ���� �������� �����մϴ�.
//
#include <windows.h>
#include <memory>
#include <cassert>
//#include "..\\RocketEngine\\IEngineProcess.h"
#include "RocketAPILoader.h"
#include "Client.h"

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/// RocketAPI�� ����´�.
	//RocketAPILoader::Instance().Initialize();

	/// Ŭ���̾�Ʈ�� �ʱ�ȭ�ϰ� �غ��Ų��.
	std::unique_ptr<Client> client(new Client());
	client->Initialize();

	/// ������ �ʱ�ȭ �ϰ� ������Ų��.
	RocketEngine::EngineProcess* rocketEngine = RocketEngine::CreateLauncher();
	//RocketEngine::IEngineProcess* rocketEngine = RocketAPILoader::Instance().CreateLauncher();
	rocketEngine->Initialize(static_cast<void*>(hInstance), nCmdShow);
	rocketEngine->RunEngine();

	/// ������ �����Ѵ�.
	rocketEngine->Finalize();

	RocketEngine::ReleaseLauncher(rocketEngine);
}

// #include "..\\RocketEngine\\IRocketLauncher.h"
// #ifdef _DEBUG
// #define ENGINEDLL_PATH L"..\\x64\\Debug\\RocketEngine.dll"
// #else
// #define ENGINEDLL_PATH L"Engine\\RocketEngine.dll" 
// #endif // _DEBUG
// #define LAUNCHER_SIGNATURE RocketEngine::IRocketLauncher* (*) (void)
// #define LAUNCHER_NAME "CreateEngine"
// main
// {
// 	HMODULE hEngineModule = LoadLibrary(ENGINEDLL_PATH);
// 
// 	assert(hEngineModule);
// 
// 	std::unique_ptr<RocketEngine::IRocketLauncher> rocketEngine((reinterpret_cast<LAUNCHER_SIGNATURE>(GetProcAddress(hEngineModule, LAUNCHER_NAME)))());
// }
