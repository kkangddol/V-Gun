#pragma once

class Transform
{
public:
	struct Position
	{
		float x;
		float y;
		float z;
	} position;
	struct Quaternion
	{
		float w;
		float x;
		float y;
		float z;
	} quaternion;

	bool isChangeFlag;

	void Init() { ::memset(this, 0, sizeof(Transform));	}
	void ReadTransform(const Protocol::Transform& transform);
	void WriteTransform(Protocol::Transform* transform);
};

