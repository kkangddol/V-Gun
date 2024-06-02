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

		//ModelData�� ������ ��, ���� ������ ���� �ؼ� �ޱ�!
		virtual void AssignData(AssetModelData* const modelData) override;

		void PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		void ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes);

		//VB �޾Ƶ��̴� �뵵.
		ByteBuffer* m_ByteBufferForVB = nullptr;
		//IB �޾Ƶ��̴� �뵵.
		std::vector<unsigned int> m_IBVec;
		//�ƴϸ�, �ٷ� �긦 ��������.
		ID3D11Buffer* m_VB = nullptr;
		ID3D11Buffer* m_IB = nullptr;
		UINT m_IndexCount = 0;

	};
}


