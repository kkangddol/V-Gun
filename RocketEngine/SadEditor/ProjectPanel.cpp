#include "ProjectPanel.h"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"

ProjectPanel::ProjectPanel()
{

}

ProjectPanel::~ProjectPanel()
{

}

void ProjectPanel::Start()
{

}

void ProjectPanel::Update()
{

}

void ProjectPanel::RenderContextMenu()
{
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("New Scene"))
		{
			// 씬 하나를 새로 생성해서 벡터에 넣는다?
		}

		ImGui::EndPopup();
	}
}
