#pragma once
#include "DataTypes.h"
#include <string>

namespace SAD
{

	class HierarchyPanel
	{
	public:
		HierarchyPanel();
		~HierarchyPanel();

	public:
		const void DrawParentNode();
		const void DrawChildrenNode(transfromData& data);

		static std::string currentNode;
	};

}
