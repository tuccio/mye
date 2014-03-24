#include "DX11Module.h"
#include "DX11VertexBuffer.h"

#include <mye/core/Game.h>

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::math;

DX11Module::DX11Module(void) :
	m_ownedDevice(false),
	m_mvp(nullptr)
{
	NewWindow();
}

DX11Module::DX11Module(DX11Window *window) :
	m_ownedDevice(false),
	m_mvp(nullptr)
{
	SetWindow(window);
}

DX11Module::~DX11Module(void)
{
	FreeWindow();
}

bool DX11Module::Init(void)
{
	m_window->Show();
	m_mvp->Create(sizeof(float) * 16, Matrix4f(1.0f).Data());
	return m_window->Init();
}

void DX11Module::ShutDown(void)
{
	m_window->Hide();
}

void DX11Module::Render(void)
{

	m_device->GetImmediateContext()->ClearRenderTargetView(
		m_window->GetRenderTargetView(),
		m_clearColor.Data()
		);

	m_device->GetImmediateContext()->ClearDepthStencilView(
		m_window->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	SceneModule *scene = Game::GetSingleton().GetSceneModule();
	CameraComponent *camera = scene->GetCamera();

	if (camera)
	{

		SceneModule::ObjectsList visibleObjects = scene->GetVisibleObjects();

		Matrix4f viewProjection = camera->GetProjectionMatrix() *
			camera->GetViewMatrix();

		for (GameObject *object : visibleObjects)
		{

			RenderComponent *rc = object->GetRenderComponent();

			if (rc)
			{

				RigidBodyComponent *rb = object->GetRigidBodyComponent();
				TransformComponent *tc = object->GetTransformComponent();

				ModelPointer model = rc->GetModel();

				if (model)
				{

					model->Load();
					m_mvp->Bind(PIPELINE_VERTEX_SHADER, 0);

					if (rb)
					{
						m_mvp->SetData((viewProjection *
							rb->GetWorldMatrix()).Data());
					}
					else
					{
						m_mvp->SetData((viewProjection *
							tc->GetWorldMatrix()).Data());
					}

					DX11VertexBuffer vertexBuffer(nullptr, "", nullptr, *m_device);

					vertexBuffer.Create(model.get());
					vertexBuffer.Bind();

					m_device->GetImmediateContext()->
						IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					m_device->GetImmediateContext()->
						Draw(vertexBuffer.GetVerticesCount(), 0);

					vertexBuffer.Destroy();

				}

			}

		}

	}

	

	m_window->GetSwapChain()->Present(1, 0);

}

void DX11Module::SetWindow(DX11Window *window)
{

	FreeWindow();

	m_device = &(window->GetDevice());
	m_window = window;
	m_ownedDevice = false;
	m_mvp = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);

	m_mainWindowPointer = m_window;

}

void DX11Module::NewWindow(void)
{

	FreeWindow();

	m_device = new DX11Device;
	m_window = new DX11Window(*m_device);
	m_ownedDevice = true;
	m_window->Create();
	m_mvp = new DX11ConstantBuffer(nullptr, "", nullptr, *m_device);

	m_mainWindowPointer = m_window;

}

void DX11Module::FreeWindow(void)
{

	if (m_ownedDevice)
	{
		delete m_window;
		delete m_device;
		delete m_mvp;
	}

	m_window = nullptr;
	m_device = nullptr;
	m_ownedDevice = false;

}