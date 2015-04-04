#pragma once

#include <d3d11.h>

#include "DX11Utils.h"

namespace mye
{

	namespace dx11
	{

		class DX11ShaderResource
		{

		public:

			DX11ShaderResource(void) : m_shaderResourceView(nullptr) { }

			inline ID3D11ShaderResourceView * GetShaderResourceView(void) const
			{
				return m_shaderResourceView;
			}

			void Bind(DX11PipelineStage stage, int slot)
			{

				ID3D11Device        * device;
				ID3D11DeviceContext * context;
				
				m_shaderResourceView->GetDevice(&device);
				device->GetImmediateContext(&context);

				auto bind = GetBindFunction(stage);

				(context->*bind)(slot, 1, &m_shaderResourceView);

				m_lastBoundSlot  = slot;
				m_lastBoundStage = stage;

				__MYE_DX11_RELEASE_COM(context);
				__MYE_DX11_RELEASE_COM(device);

			}

			void Unbind(void)
			{
					
				ID3D11Device        * device;
				ID3D11DeviceContext * context;

				ID3D11ShaderResourceView * unbound = nullptr;

				m_shaderResourceView->GetDevice(&device);
				device->GetImmediateContext(&context);

				auto bind = GetBindFunction(m_lastBoundStage);

				(context->*bind)(m_lastBoundSlot, 1, &unbound);

				__MYE_DX11_RELEASE_COM(context);
				__MYE_DX11_RELEASE_COM(device);

			}

			inline void GenerateMips(void)
			{

				ID3D11Device        * device;
				ID3D11DeviceContext * context;

				m_shaderResourceView->GetDevice(&device);
				device->GetImmediateContext(&context);

				context->GenerateMips(m_shaderResourceView);

				__MYE_DX11_RELEASE_COM(context);
				__MYE_DX11_RELEASE_COM(device);

			}

		protected:

			ID3D11ShaderResourceView * m_shaderResourceView;

			int                        m_lastBoundSlot;
			DX11PipelineStage          m_lastBoundStage;

		private:

			inline auto GetBindFunction(DX11PipelineStage stage) -> decltype(&ID3D11DeviceContext::PSSetShaderResources)
			{
				switch (stage)
				{

				case DX11PipelineStage::GEOMETRY_SHADER:
					return &ID3D11DeviceContext::GSSetShaderResources;

				case DX11PipelineStage::VERTEX_SHADER:
					return &ID3D11DeviceContext::VSSetShaderResources;

				case DX11PipelineStage::PIXEL_SHADER:
					return &ID3D11DeviceContext::PSSetShaderResources;

				case DX11PipelineStage::COMPUTE_SHADER:
					return &ID3D11DeviceContext::CSSetShaderResources;

				default:

					return nullptr;


				}
			}


		};

	}
}