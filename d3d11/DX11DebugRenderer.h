#pragma once

#include "DX11Device.h"

#include "DX11SwapChain.h"
#include "DX11Texture.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

#include <mye/win/Window.h>

#include <vector>

namespace mye
{

	namespace dx11
	{

		class DX11DebugRenderer
		{

		public:

			DX11DebugRenderer(void);
			~DX11DebugRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView * target);

			void EnqueueShaderResource(DX11ShaderResource & shaderResource, const mye::math::Vector2i & position, const mye::math::Vector2i & size, int slice = 0);
			void EnqueueLine(const mye::math::Vector3 & from, const mye::math::Vector3 & to, const mye::math::Vector4 & color);
			void EnqueueFrustum(const mye::math::Frustum & frustum, const mye::math::Vector4 & color);

		private:

			struct __ShadowResource
			{
				DX11ShaderResource * shaderResource;
				int                  position[2];
				float                size[2];
				int                  slice;
			};

			struct __Line
			{
				float from[3];
				float to[3];
				float color[4];
			};

			bool                            m_initialized;
									      
			DX11VertexShaderPointer         m_texturedSimpleVS;
			DX11PixelShaderPointer          m_texturedSimplePS;
			DX11PixelShaderPointer          m_texturedArrayPS;
									      
			DX11VertexShaderPointer         m_primitiveVS;
			DX11PixelShaderPointer          m_primitivePS;
									      
			ID3D11SamplerState            * m_linearSampler;

			std::vector<__ShadowResource>   m_shaderResources;
			std::vector<__Line>             m_lines;

		};

	}

}
