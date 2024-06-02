#include "Texture.h"
#include "Define3D.h"
#include "RenderPipeline.h"
#include "LowLevelDX11.h"

namespace RocketCore::Graphics
{
	Texture::Texture()
	{
		//
	}
	///나중에는 CreateResourceView를 통해 Resource를 경로 따로 만들어둔 뒤 배정하고 싶다.
	void Texture::AssignSRVFromResource(ID3D11Resource* _resource)
	{
		HRESULT hr = S_OK;
		
		//이때, D3D11_BIND_SHADER_RESOURCE로 만들어진 Resource여야!
		hr = LowLevelDX11::GetInstance()->m_d3dDevice.Get()->
			CreateShaderResourceView(_resource, NULL, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
	}

	ID3D11Resource* Texture::AssignSRVFromDDSPath(const std::wstring& _path)
	{
		HRESULT hr = S_OK;

		ID3D11Resource* tmpRes = nullptr;
		
		hr = CreateDDSTextureFromFile(
			LowLevelDX11::GetInstance()->m_d3dDevice.Get(),
			NULL, _path.c_str(), &tmpRes, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
		else
		{
			std::wstring tBasicError = L"AssignSRVFromDDSPath 중 오류 - 인식 시도 Path : ";
			tBasicError.append(_path);
			MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"오류...", MB_ICONERROR | MB_OK);
		}
		return tmpRes;
	}

	ID3D11Resource* Texture::AssignSRVFromWICPath(const std::wstring& _path)
	{
		ID3D11Resource* tmpRes = nullptr;

		HRESULT hr = S_OK;
		hr = CreateWICTextureFromFile(
			LowLevelDX11::GetInstance()->m_d3dDevice.Get(),
			LowLevelDX11::GetInstance()->m_d3dMainContext.Get(),
			_path.c_str(), &tmpRes, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
		else
		{
			std::wstring tBasicError = L"AssignSRVFromWICPath 중 오류 - 인식 시도 Path : ";
			tBasicError.append(_path);
			MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"오류...", MB_ICONERROR | MB_OK);
		}

		return tmpRes;

	}

	void Texture::SetSRV(ID3D11ShaderResourceView* _srv)
	{
		m_SRV = _srv;
	}

	ID3D11ShaderResourceView* Texture::GetSRV()
	{
		return m_SRV;
	}





}
