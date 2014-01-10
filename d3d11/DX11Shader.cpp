#include "DX11Shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>

using namespace mye::dx11;

#define TARGET_VERTEX_SHADER "vs_5_0"
#define TARGET_PIXEL_SHADER "ps_5_0"
#define TARGET_EFFECTS_SHADER "fx_5_0"

DX11Shader::DX11Shader(DX11Device &device) :
	m_device(device)
{
}


DX11Shader::~DX11Shader(void)
{
}

bool DX11Shader::Compile(const std::string &src,
						 Type type,
						 std::string &infoLog)
{

	std::auto_ptr<WCHAR> wString(new wchar_t[src.length() + 1]);
	mbstowcs(&*wString, src.c_str(), src.length());

	const char *target;

	switch (type)
	{
	case SHADERTYPE_PIXEL:
		target = TARGET_PIXEL_SHADER;
		break;
	case SHADERTYPE_VERTEX:
		target = TARGET_VERTEX_SHADER;
		break;
	case SHADERTYPE_EFFECTS:
		target = TARGET_EFFECTS_SHADER;
		break;
	}

	UINT compileFlags = 0x0;

#ifdef DEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob *code, *error;

	HRESULT hResult = D3DCompileFromFile(
		&*wString,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		NULL,
		target,
		compileFlags,
		0x0,
		&code,
		&error);

	if (FAILED(hResult))
	{
		infoLog = (LPCSTR) error->GetBufferPointer();
		return false;
	}


	switch (type)
	{

	case SHADERTYPE_VERTEX:

		m_device.GetDevice()->CreateVertexShader(
			code->GetBufferPointer(),
			code->GetBufferSize(),
			NULL,
			&m_shader.vertex);

		break;

	case SHADERTYPE_PIXEL:

		m_device.GetDevice()->CreatePixelShader(
			code->GetBufferPointer(),
			code->GetBufferSize(),
			NULL,
			&m_shader.pixel);

		break;

	case SHADERTYPE_EFFECTS:

		m_device.GetDevice()->CreatePixelShader(
			code->GetBufferPointer(),
			code->GetBufferSize(),
			NULL,
			&m_shader.pixel);

		break;

	};

	return true;	

}