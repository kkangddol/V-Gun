#pragma once
#include<vector>
/// <summary>
/// 23.08.03 ������
/// ���� �����ϰ� �����ϴ� �г�
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

