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
			// �� �ϳ��� ���� �����ؼ� ���Ϳ� �ִ´�?
		}

		ImGui::EndPopup();
	}
}
