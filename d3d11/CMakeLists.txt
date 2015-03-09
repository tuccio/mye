cmake_minimum_required(VERSION 3.0)

project (myed3d11)

add_library ( myed3d11 BasicRenderer.cpp BasicRenderer.h DeferredLightingRenderer.cpp DeferredLightingRenderer.h DX11Buffer.cpp DX11Buffer.h DX11BufferManager.cpp DX11BufferManager.h DX11ConstantBuffer.cpp DX11ConstantBuffer.h DX11DepthBuffer.cpp DX11DepthBuffer.h DX11Device.cpp DX11Device.h DX11Font.cpp DX11Font.h DX11FontManager.cpp DX11FontManager.h DX11Module.cpp DX11Module.h DX11PixelShader.cpp DX11PixelShader.h DX11RasterizerState.cpp DX11RasterizerState.h DX11Shader.cpp DX11Shader.h DX11ShaderManager.cpp DX11ShaderManager.h DX11SwapChain.cpp DX11SwapChain.h DX11Texture.cpp DX11Texture.h DX11TextureManager.cpp DX11TextureManager.h DX11Utils.cpp DX11Utils.h DX11VertexBuffer.cpp DX11VertexBuffer.h DX11VertexShader.cpp DX11VertexShader.h Text2DRenderer.cpp Text2DRenderer.h )
