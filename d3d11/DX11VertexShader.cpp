#include "DX11VertexShader.h"
#include "DX11Utils.h"

#include <d3dcompiler.h>
#include <mye/core/Utils.h>

#include <sstream>

using namespace mye::dx11;

DX11VertexShader::DX11VertexShader(mye::core::ResourceManager *owner,
								   const std::string &name,
								   mye::core::ManualResourceLoader *manual,
								   DX11Device &device) :
DX11Shader(owner, name, manual),
	m_device(device)
{
	m_shader = NULL;
	m_inputLayout = NULL;
}

void DX11VertexShader::Use(void)
{
	m_device.GetImmediateContext()->VSSetShader(m_shader, NULL, 0);
	m_device.GetImmediateContext()->IASetInputLayout(m_inputLayout);
}


DX11VertexShader::~DX11VertexShader(void)
{
}

ID3D11VertexShader* DX11VertexShader::GetVertexShader(void)
{
	return m_shader;
}

std::string DX11VertexShader::GetCompileError(void)
{
	return m_compileError;
}


bool DX11VertexShader::LoadImpl(void)
{

	bool success = false;

	if (DX11Shader::LoadImpl())
	{
		
		ID3DBlob *code, *error;
		UINT compileFlags = 0x0;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		
		if (HRTESTFAILED(D3DCompile(
			m_source.c_str(),
			m_source.length(),
			m_name.c_str(),
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			compileFlags,
			0x0,
			&code,
			&error)))
		{
			m_compileError = (LPCSTR) error->GetBufferPointer();
		}
		else if (!HRTESTFAILED(m_device.GetDevice()->CreateVertexShader(
			code->GetBufferPointer(),
			code->GetBufferSize(),
			NULL,
			&m_shader)))
		{

			auto it = m_params.find("inputLayoutVector");

			if (it != m_params.end())
			{

				void *ptr = mye::core::StringToPointer(it->second);

				std::vector<D3D11_INPUT_ELEMENT_DESC>* vDesc = static_cast<std::vector<D3D11_INPUT_ELEMENT_DESC>*>(ptr);

				if (!HRTESTFAILED(m_device.GetDevice()->
					CreateInputLayout(
					&vDesc->front(),
					vDesc->size(),
					code->GetBufferPointer(),
					code->GetBufferSize(),
					&m_inputLayout)))
				{
					success = true;
				}

			}
			else
			{
				success = true;
			}

		}

	}

	if (!success)
	{
		Destroy();
	}

	return success;

}

void DX11VertexShader::UnloadImpl(void)
{

	if (m_shader)
	{
		ReleaseCOM(m_shader);
	}

	m_compileError.clear();

}

size_t DX11VertexShader::CalculateSizeImpl(void)
{
	return 0;
}

void DX11VertexShader::Destroy(void)
{

	if (m_shader)
	{
		ReleaseCOM(m_shader);
	}
	
	m_compileError.clear();

}