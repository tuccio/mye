#pragma once

#include <mye/core/Image.h>
#include <mye/core/Texture.h>
#include <mye/core/VertexDeclaration.h>

#include <d3d11.h>

#include "DX11Device.h"

namespace mye
{

	namespace dx11
	{

		/*
		 * Params:
		 *
		 *    int    levels       : Mimap levels
		 *    bool   multisample  : Antialiasing setting
		 *    string usage        : [default/static/dynamic/stream] -> [rw gpu/r gpu/r gpu w cpu/rw gpu cpu]
		 *    bool  generateMips  : Mipmap generation setting
		 *
		 */

		class DX11Texture :
			public mye::core::Texture
		{

		public:

			DX11Texture(mye::core::ResourceManager *owner,
				const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				DX11Device &device);

			~DX11Texture(void);

			void Bind(unsigned int slot);
			void Unbind(void);

			bool Create(int width, int height, mye::core::DataFormat format);
			void Destroy(void);

			void ClearRenderTarget(const mye::math::Vector4f &color);

			inline ID3D11RenderTargetView*   GetRenderTargetView(void);
			inline ID3D11ShaderResourceView* GetShaderResourceView(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			DX11Device      &m_device;
			ID3D11Texture2D *m_texture;

			ID3D11ShaderResourceView *m_shaderResourceView;
			ID3D11RenderTargetView   *m_renderTargetView;

			mye::core::DataFormat m_format;
			int                   m_boundSlot;

			bool CreateViews(void);

		};

		typedef boost::shared_ptr<DX11Texture> DX11TexturePointer;

		ID3D11RenderTargetView* DX11Texture::GetRenderTargetView(void)
		{
			return m_renderTargetView;
		}

		ID3D11ShaderResourceView* DX11Texture::GetShaderResourceView(void)
		{
			return m_shaderResourceView;
		}

	}

}

