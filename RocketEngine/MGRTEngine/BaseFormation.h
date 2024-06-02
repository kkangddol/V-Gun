#pragma once
#include <vector>
#include <string>
#include "TypedefGraphicHelper.h"

struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace RocketCore::Graphics
{
	class AssetModelData;

	class BaseFormation
	{
	public:
		BaseFormation(const AssetInputStruct& inputStruct, AssetModelData* modelData);
		virtual ~BaseFormation();
		//ModelData�� ������ ��, ���� ������ ���� �ؼ� �ޱ�!
		virtual void AssignData(AssetModelData* const modelData) abstract;
		
		ID3D11InputLayout* GetInputLayout();
		void SetInputLayout(ID3D11InputLayout* layout);
		unsigned int GetSingleBufferSize();
		void SetSingleBufferSize(unsigned int size);
		unsigned int GetLayoutCount();
		void SetLayoutCount(unsigned int cnt);
		AssetModelData* GetModelData();
		void SetModelData(AssetModelData* modelData);

		template <typename T>
		T* GetDerivedSelf();
		//���� �а� ���� ���� �ٿ�ĳ����, dynamic_cast�� üũ.

	protected:
		ID3D11InputLayout* _inputLayout = nullptr;
		//Vertex �ϳ��� ����Ʈ ũ��.
		unsigned int _singleBufferSize = NULL;
		//m_OffsetVec�� Size�� �������´�. (�� ���� �������� ����)
		unsigned int _layoutCount = NULL;

		// Offset, Stride ����.
		std::vector<uint32_t> _offsetVec;

		//InputLayoutStruct���� Semantic String ����Ʈ�� ���� ����!
		std::vector<std::string> _semanticNameVec;

		AssetModelData* _modelData = nullptr; //������ ������ �ϵ�, ��ü������ RU�� VB/IB�� �ε��ϴ� ��� �Լ��� ����.
	};

	template <typename T>
	T* RocketCore::Graphics::BaseFormation::GetDerivedSelf()
	{
		T* tDerived = dynamic_cast<T*>(this);
		assert(tDerived != nullptr);
		return tDerived;
	}

}

