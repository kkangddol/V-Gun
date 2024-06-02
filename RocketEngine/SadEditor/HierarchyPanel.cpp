#include "HierarchyPanel.h"

#include "Serialize.h"
namespace SAD
{
	std::string HierarchyPanel::currentNode;

	HierarchyPanel::HierarchyPanel()
	{

	}

	HierarchyPanel::~HierarchyPanel()
	{

	}

	const void HierarchyPanel::DrawParentNode()
	{
		for (const auto& data : Serialize::serializeData)
		{
			std::string test = data.transform->parent;

			if (data.transform->parent == "")
			{
				if (data.transform->child.empty())
				{
					if (ImGui::Selectable(data.transform->name.c_str(), data.transform->isNodeOpen))
					{
						currentNode = data.transform->name;
					}
				}
				else
				{
					if (ImGui::TreeNodeEx(data.transform->name.c_str(), (currentNode == data.transform->name ? ImGuiTreeNodeFlags_Selected : 0)))
					{
						if (ImGui::IsItemToggledOpen())
						{
							currentNode = data.transform->name;
						}
						if (!ImGui::IsItemClicked())
						{
							data.transform->isNodeOpen = !data.transform->isNodeOpen;
						}
						DrawChildrenNode(*data.transform);
						ImGui::TreePop();
					}
				}
			}

		}
	}

	const void HierarchyPanel::DrawChildrenNode(transfromData& data)
	{
		for (const auto& child : data.child)
		{
			for (const auto& allData : Serialize::serializeData)
			{
				if (!(strcmp(child.c_str(), allData.transform->name.c_str()) == 0))
					continue;

				if (allData.transform->child.empty())
				{
					if (ImGui::Selectable(allData.transform->name.c_str(), allData.transform->isNodeOpen))
					{
						currentNode = allData.transform->name;
					}
				}
				else
				{
					if (ImGui::TreeNodeEx(allData.transform->name.c_str(), (currentNode == allData.transform->name ? ImGuiTreeNodeFlags_Selected : 0)))
					{
						if (ImGui::IsItemToggledOpen())
						{
							currentNode = allData.transform->name;
						}
						if (!ImGui::IsItemClicked())
						{
							allData.transform->isNodeOpen = !allData.transform->isNodeOpen;
						}
						DrawChildrenNode(*allData.transform);
						ImGui::TreePop();
					}
				}
			}
		}
	}
}