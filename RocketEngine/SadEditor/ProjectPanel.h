#pragma once
#include<vector>
/// <summary>
/// 23.08.03 오수안
/// 씬을 생성하고 관리하는 패널
/// </summary>

class ProjectPanel
{
public:
	ProjectPanel();
	~ProjectPanel();

public:
	void Start();
	void Update();

private:
	//std::vector<Scene*> _myScenes;
	void RenderContextMenu();

};

