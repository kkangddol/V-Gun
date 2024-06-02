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
		//ModelData가 들어왔을 때, 내부 구현과 따로 해서 받기!
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
		//버퍼 읽고 쓰는 것은 다운캐스팅, dynamic_cast로 체크.

	protected:
		ID3D11InputLayout* _inputLayout = nullptr;
		//Vertex 하나의 바이트 크기.
		unsigned int _singleBufferSize = NULL;
		//m_OffsetVec의 Size를 결정짓는다. (몇 개를 기준으로 할지)
		unsigned int _layoutCount = NULL;

		// Offset, Stride 보관.
		std::vector<uint32_t> _offsetVec;

		//InputLayoutStruct에서 Semantic String 리스트를 빼서 보관!
		std::vector<std::string> _semanticNameVec;

		AssetModelData* _modelData = nullptr; //포인터 저장은 하되, 자체적으로 RU에 VB/IB를 로드하는 멤버 함수가 있음.
	};

	template <typename T>
	T* RocketCore::Graphics::BaseFormation::GetDerivedSelf()
	{
		T* tDerived = dynamic_cast<T*>(this);
		assert(tDerived != nullptr);
		return tDerived;
	}

}

