#include "ScreenBufferPrecombined.h"
#include <d3d11.h>
#include "Shader.h"
namespace RocketCore::Graphics
{
	void ScreenBufferPrecombined::AssignRequiredPrecombinedBufferVariables(
		const std::string& nearClip, const std::string& farClip,
		const std::string& forwardQuadBuf, const std::string& forwardDepthBuf, 
		const std::string& deferredPhongQuadBuf, const std::string& deferredPhongDepthBuf, 
		const std::string& deferredPBRQuadBuf, const std::string& deferredPBRDepthBuf, 
		const std::string& deferredPBRSkinnedQuadBuf, const std::string& deferredPBRSkinnedDepthBuf)
	{
		_gNearClipEffectVar = this->_belongShader->GetEffect()->GetVariableByName(nearClip.c_str())->AsScalar();
		assert(_gNearClipEffectVar->IsValid());
		
		_gFarClipEffectVar = this->_belongShader->GetEffect()->GetVariableByName(farClip.c_str())->AsScalar();
		assert(_gFarClipEffectVar->IsValid());

		_gForward_QuadBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(forwardQuadBuf.c_str())->AsShaderResource();
		assert(_gForward_QuadBufferEffectVar->IsValid());

		_gForward_DepthBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(forwardDepthBuf.c_str())->AsShaderResource();
		assert(_gForward_DepthBufferEffectVar->IsValid());

		_gDeferredPhong_QuadBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPhongQuadBuf.c_str())->AsShaderResource();
		assert(_gDeferredPhong_QuadBufferEffectVar->IsValid());

		_gDeferredPhong_DepthBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPhongDepthBuf.c_str())->AsShaderResource();
		assert(_gDeferredPhong_DepthBufferEffectVar->IsValid());

		_gDeferredPBR_QuadBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPBRQuadBuf.c_str())->AsShaderResource();
		assert(_gDeferredPBR_QuadBufferEffectVar->IsValid());

		_gDeferredPBR_DepthBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPBRDepthBuf.c_str())->AsShaderResource();
		assert(_gDeferredPBR_DepthBufferEffectVar->IsValid());

		_gDeferredPBR_Skinned_QuadBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPBRSkinnedQuadBuf.c_str())->AsShaderResource();
		assert(_gDeferredPBR_Skinned_QuadBufferEffectVar->IsValid());

		_gDeferredPBR_Skinned_DepthBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(deferredPBRSkinnedDepthBuf.c_str())->AsShaderResource();
		assert(_gDeferredPBR_Skinned_DepthBufferEffectVar->IsValid());
	}

}