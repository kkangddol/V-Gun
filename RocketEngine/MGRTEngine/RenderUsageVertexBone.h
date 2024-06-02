#pragma once
#include <vector>
namespace RocketCore::Graphics
{
	class RenderUsageVertexBone
	{
	public:
		RenderUsageVertexBone();
		void AddBoneData(unsigned int BoneID, float Weight);
		bool isInit = false;

		//�̰� ���� Vertex Array�� �ݿ����� �������� ���� �ȴ�.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};
}


