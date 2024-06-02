#include "BaseFormation.h"
#include "AssetModelData.h"
#include <cassert>

namespace RocketCore::Graphics
{
	BaseFormation::BaseFormation(const AssetInputStruct& inputStruct, AssetModelData* modelData) :
		_inputLayout(inputStruct.m_Layout), _layoutCount(inputStruct.m_InputLayoutCount), _singleBufferSize(inputStruct.m_Strides[0])
	{
		_offsetVec.reserve(_layoutCount);
		for (int i = 0; i < _layoutCount; i++)
		{
			_offsetVec.push_back(inputStruct.m_Offsets[i]);
		}

		//Semantic Name Vector บนป็.
		this->_semanticNameVec = inputStruct.m_SemanticNameVector;

		this->_modelData = modelData;
	}

	BaseFormation::~BaseFormation()
	{
		//
	}

	ID3D11InputLayout* BaseFormation::GetInputLayout()
	{
		assert(_inputLayout != nullptr);
		return _inputLayout;
	}

	void BaseFormation::SetInputLayout(ID3D11InputLayout* layout)
	{
		assert(layout != nullptr);
		this->_inputLayout = layout;
	}

	unsigned int BaseFormation::GetSingleBufferSize()
	{
		assert(_singleBufferSize != NULL);
		return _singleBufferSize;
	}

	void BaseFormation::SetSingleBufferSize(unsigned int size)
	{
		assert(size != NULL);
		this->_singleBufferSize = size;
	}

	unsigned int BaseFormation::GetLayoutCount()
	{
		assert(_layoutCount != NULL);
		return _layoutCount;
	}

	void BaseFormation::SetLayoutCount(unsigned int cnt)
	{
		assert(cnt != NULL);
		this->_layoutCount = cnt;
	}

	RocketCore::Graphics::AssetModelData* BaseFormation::GetModelData()
	{
		assert(_modelData != nullptr);
		return _modelData;
	}

	void BaseFormation::SetModelData(AssetModelData* modelData)
	{
		assert(modelData != nullptr);
		this->_modelData = modelData;
	}

	

}	