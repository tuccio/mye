#include "DX11Module.h"
#include "DX11VertexBuffer.h"
#include "DX11Font.h"
#include "DX11RasterizerState.h"
#include "DX11ShaderManager.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "./detail/ShadersBuffers.h"

#include <random>

#define __MYE_DX11_RENDERER m_deferredLightingRenderer

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::math;
using namespace mye::win;

DX11Module::DX11Module(void) :
	m_window(nullptr),
	m_stopWatchBufferHead(0)
{
	SetWindow(nullptr);
}

DX11Module::DX11Module(Window * window) :
	m_window(nullptr),
	m_stopWatchBufferHead(0)
{

	SetWindow(window);

}

DX11Module::~DX11Module(void)
{
	FreeWindow();
}

bool DX11Module::Init(void)
{

	if (m_window)
	{
		m_window->Show();
	}

	if ((m_device.Exists() || m_device.Create()) &&
		(m_swapChain.Exists() || m_swapChain.Create()))
	{

		OnResize(m_window, m_window->GetSize());

		DX11RasterizerState rasterizeState({ false, CullMode::NONE });

		rasterizeState.Use();

		__CreateSharedResources();

		if (__MYE_DX11_RENDERER.Init() &&
			m_text2dRenderer.Init() &&
			m_debugRenderer.Init())
		{
			m_stopWatch.Start();
			Module::Init();
		}

	}

	return m_initialized;

}

void DX11Module::Shutdown(void)
{

	m_stopWatch.Stop();
	__MYE_DX11_RENDERER.Shutdown();
	m_text2dRenderer.Shutdown();
	m_debugRenderer.Shutdown();
	
	FreeWindow();

	m_swapChain.Destroy();
	m_device.Destroy();

}

void DX11Module::Render(void)
{

	if (m_resizeSwapChain)
	{

		auto size = GetRendererConfiguration()->GetScreenResolution();

		m_swapChain.Resize(size.x(), size.y());

		if (m_device.Exists())
		{

			D3D11_VIEWPORT viewPort;

			viewPort.MinDepth = 0.f;
			viewPort.MaxDepth = 1.f;
			viewPort.TopLeftX = 0.f;
			viewPort.TopLeftY = 0.f;
			viewPort.Width    = (float) size.x();
			viewPort.Height   = (float) size.y();

			m_device.GetImmediateContext()->RSSetViewports(1, &viewPort);

		}

		m_resizeSwapChain = false;

	}

	auto backBuffer = m_swapChain.GetBackBufferRenderTargetView();

	m_swapChain.ClearBackBuffer(m_clearColor);

	__MYE_DX11_RENDERER.Render(backBuffer);

	//m_text2dRenderer.Render(backBuffer);
	//m_debugRenderer.Render(backBuffer);

	m_swapChain->Present((m_vsync ? 1 : 0), 0);
	
	m_stopWatchBuffer[m_stopWatchBufferHead] = m_stopWatch.Lap();
	m_stopWatchBufferHead = (m_stopWatchBufferHead + 1) % __MYE_FPS_COUNTER_BUFFER_SIZE;

}

void DX11Module::SetWindow(Window * window)
{

	FreeWindow();

	if (window)
	{

		m_window = window;
		m_mainWindowPointer = static_cast<IWindow*>(m_window);

		m_window->AddListener(static_cast<IWindow::Listener*>(this));

		auto clientSize = window->GetSize();

		DX11SwapChainConfiguration swapChainConf;

		swapChainConf.window     = m_window;
		swapChainConf.format     = DataFormat::sRGBA32;
		swapChainConf.fullscreen = window->IsFullScreen();
		swapChainConf.height     = clientSize.y();
		swapChainConf.width      = clientSize.x();
		swapChainConf.msaa       = MSAA::MSAA_OFF;
		swapChainConf.refresh    = mye::math::Rational<unsigned int>(1, 60);

		m_swapChain = DX11SwapChain(swapChainConf);
		
		if (DX11Device::GetSingleton().Exists())
		{

			m_swapChain.Create();
			OnResize(window, clientSize);

		}

	}

	

}

void DX11Module::FreeWindow(void)
{

	if (m_window)
	{

		m_window->RemoveListener(this);

		m_window = nullptr;
		m_mainWindowPointer = nullptr;

		m_swapChain.Destroy();

	}

}

void DX11Module::OnResize(IWindow * window, const mye::math::Vector2i & size)
{

	m_resizeSwapChain = true;

	GetRendererConfiguration()->SetScreenResolution(size);

	/*RECT rect = { 0, 0, size.x(), size.y() };
	ClipCursor(&rect);*/

}

float DX11Module::GetFPS(void) const
{

	Milliseconds t = 0;

	for (int i = 0; i < __MYE_FPS_COUNTER_BUFFER_SIZE; i++)
	{
		t = t + m_stopWatchBuffer[i];
	}

	return (1000 * __MYE_FPS_COUNTER_BUFFER_SIZE) / (float) t;

}

void DX11Module::RenderShaderResource(DX11ShaderResource & resource, const Vector2i & position, const Vector2i & size, int slice)
{
	m_debugRenderer.EnqueueShaderResource(resource, position, size, slice);
}

void DX11Module::RenderFrustum(const Frustum & frustum, const Vector4 & color)
{
	m_debugRenderer.EnqueueFrustum(frustum, color);
}

void DX11Module::__CreateSharedResources(void)
{

	static ManualLambdaLoader cosSinLoader(
		std::function<bool(Resource*)>(
			[] (Resource * r)
		{
			
			DX11Texture * t = static_cast<DX11Texture*>(r);

			const unsigned int resolution = 256;

			struct __CosSin
			{
				float cosAlpha;
				float sinAlpha;
			};

			std::vector<__CosSin> data(resolution * resolution);
			
			std::random_device                    device;
			std::mt19937                          generator(device());
			std::uniform_real_distribution<float> distribution(0.f, TwoPi<float>());

			for (auto & e : data)
			{
				float alpha = distribution(generator);
				e.cosAlpha  = Cosine(alpha);
				e.sinAlpha  = Sine(alpha);
			}

			return t->Create(resolution, resolution, DataFormat::FLOAT2, &data[0]);

		}),

		std::function<void(Resource*)>(
			[] (Resource * r)
		{
			DX11Texture * t = static_cast<DX11Texture*>(r);
			t->Destroy();
		}));

	static ManualLambdaLoader quadBufferLoader(
		std::function<bool(Resource*)>(
			[] (Resource * r)
		{
			
			DX11VertexBuffer * vb = static_cast<DX11VertexBuffer *>(r);
			
			float quad[] = {
				-1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f,
				-1.0f, 1.0f, 0.0f, 1.0f
			};

			return vb->Create(quad, 6, VertexDeclaration({ VertexAttribute(VertexAttributeSemantic::POSITION, DataFormat::FLOAT4) }));

		}),

		std::function<void(Resource*)>(
			[] (Resource * r)
		{
			DX11VertexBuffer * vb = static_cast<DX11VertexBuffer*>(r);
			vb->Destroy();
		}));

	ResourceTypeManager::GetSingleton().CreateResource<DX11Texture>("Texture",
	                                                                "MYE_RANDOM_COS_SIN",
	                                                                &cosSinLoader);

	ResourceTypeManager::GetSingleton().CreateResource<DX11VertexBuffer>("GPUBuffer",
	                                                                     "MYE_QUAD",
	                                                                     &quadBufferLoader,
	                                                                     { { "type", "vertex" } });

}