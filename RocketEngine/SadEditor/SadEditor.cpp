#define WL_PLATFORM_WINDOWS
#include <vector>
#include <unordered_map>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "../RocketEngine/RocketAPI.h"
//#include "../RocketMath/RocketMath.h"
#include "../EngineDemo/CameraMove.h"
#include "../RocketEngine/IComponent.h"
#include "../RocketEngine/DLLExporter.h"
#include "TestSceneView.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "ScenePanel.h"
#include "EditorManager.h"

#ifdef _DEBUG
#pragma comment(lib,"RocketEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketEngine.lib")
#endif // _DEBUG

#pragma comment(lib, "RocketRendering.lib")

// imgui 헤더
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "imgui_internal.h"

#include "Serialize.h"

namespace SAD
{
	// 테스트용 더미 씬
	//TestSceneView sv;

	// 엔진 프로세스 가져오기
	RocketEngine::EngineProcess* rocketEngine = RocketEngine::CreateLauncher();

	Serialize* serialize = new Serialize;
	HierarchyPanel hierarchyPanel;
	InspectorPanel inspectorPanel;
	ScenePanel scenePanel;

	class SadScene : public Walnut::Layer
	{
	public:
		virtual void OnUIRender() override
		{
			ImGui::Begin("Scene");
			
			if (EditorManager::isSceneLoad == true)
			{
				if (!EditorManager::init && ImGui::FindWindowByName("Scene")->Viewport->PlatformHandleRaw && ImGui::FindWindowByName("Scene")->Viewport->PlatformHandleRaw != (void*)glfwGetWin32Window(Walnut::Application::Get().GetWindowHandle()))
				{
					scenePanel.SceneInitialize();
					rocketEngine->InitializeForEditor(ImGui::FindWindowByName("Scene")->Viewport->PlatformHandleRaw);
					EditorManager::init = true;
				}
				if (EditorManager::init)
				{
					scenePanel.SceneRun();
					scenePanel.SceneInput();
					rocketEngine->RunCycleOnceForEditor();
				}
			}
			ImGui::End();
		}
	};

	std::string serializeData;

	class Hierarchy : public Walnut::Layer
	{
	public:
		virtual void OnUIRender() override
		{
			ImGui::Begin("Hierarchy");

			if (!((Serialize::filepath == "") || (Serialize::filepath == "error")) && EditorManager::isDrawAll == false)
			{
				serialize->GetAllDataFromFile();
				EditorManager::isSceneLoad = true;
				EditorManager::isDrawAll = true;
			}
			if (EditorManager::isSceneLoad == true)
			{
				hierarchyPanel.DrawParentNode();
			}

			ImGui::End();
		}
	};

	class Inspector : public Walnut::Layer
	{
	public:
		virtual void OnUIRender() override
		{
			ImGui::Begin("Inspector");
			if (HierarchyPanel::currentNode != "")
			{
				inspectorPanel.DrawInspector();
			}
			ImGui::End();
		}
	};
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	//SAD::sv.Initialize();
	//SAD::serialize->SerializeSceneByName(*SAD::sv.scene);


	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<SAD::Hierarchy>();
	app->PushLayer<SAD::Inspector>();
	app->PushLayer<SAD::SadScene>();

	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					SAD::EditorManager::SaveThisSceneToEngine();
				}
				if (ImGui::MenuItem("Load Scene"))
				{
					SAD::EditorManager::SetNewPath();
				}
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}

