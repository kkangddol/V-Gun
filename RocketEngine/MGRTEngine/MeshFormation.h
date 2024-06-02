#pragma once
#include "BaseFormation.h"
#include "TypedefGraphicHelper.h"

namespace RocketCore::Graphics
{
	class MeshFormation : public BaseFormation
	{
	public:
		MeshFormation(const AssetInputStruct& _inputStruct, AssetModelData* _modelData);
		virtual ~MeshFormation();

		//ModelData가 들어왔을 때, 내부 구현과 따로 해서 받기!
		virtual void AssignData(AssetModelData* const modelData) override;

		void PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		void ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes);

		//VB 받아들이는 용도.
		ByteBuffer* m_ByteBufferForVB = nullptr;
		//IB 받아들이는 용도.
		std::vector<unsigned int> m_IBVec;
		//아니면, 바로 얘를 때려박자.
		ID3D11Buffer* m_VB = nullptr;
		ID3D11Buffer* m_IB = nullptr;
		UINT m_IndexCount = 0;

	};
}


