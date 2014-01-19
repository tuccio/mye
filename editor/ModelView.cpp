#include "ModelView.h"
#include "Globals.h"

#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11Utils.h>

#include <mye/win/Utils.h>

#include <DirectXMath.h>

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::win;

#define MVIEW_OPEN 0x0
#define MVIEW_TUNE 0x1

ModelView::ModelView(mye::dx11::DX11Device &device) :
	m_window(device),
	m_device(device),
	m_initialized(false),
	m_toolbar(g_mainWindow, true),
	m_model(NULL, "", &m_modelLoader),
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_vbuffer(NULL, "", NULL, device),
	m_mvpBuffer(NULL, "", NULL, device),
	m_transform(Eigen::Matrix4f::Identity())
{
	m_toolbar.SetIconSize(Eigen::Vector2i(24, 24));
	m_window.SetMSAA(DX11Window::MSAA_4x);
}


ModelView::~ModelView(void)
{
	m_vbuffer.Destroy();
}

mye::core::Model& ModelView::GetModel(void)
{
	return m_model;
}

void ModelView::Activate(void)
{

	if (!m_initialized)
	{

		Window::Properties p;

		p.caption    = "Model View";
		p.fullscreen = false;
		p.width      = -1;
		p.height     = -1;
		p.x          = -1;
		p.y          = -1;

		m_window.CreateChild(g_mainWindow, p);

		if (!m_window.Init())
		{
			ShowErrorBox("Error while initiating rendering window\nConsult logs for more details");
			exit(1);
		}

		m_toolbar.AddButton("icons/folder.bmp",
			[this]()->void
		{

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));

			char buffer[256] = { 0 };

			ofn.lStructSize     = sizeof(OPENFILENAME);
			ofn.hwndOwner       = g_mainWindow.GetHandle();
			ofn.lpstrFile       = buffer;
			ofn.nMaxFile        = sizeof(buffer);
			ofn.lpstrFilter     = "Wavefront OBJ\0*.obj";
			ofn.nFilterIndex    = 1;
			ofn.lpstrFileTitle  = NULL;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			GetOpenFileName(&ofn);

			ResourceHandle hMesh = ResourceTypeManager::GetSingleton().
				CreateResource("Mesh", buffer);

			hMesh.get()->Load();

			this->GetModel().AddMesh(hMesh.get()->GetName());

		},
			"Open");

		m_toolbar.AddButton("icons/gear.bmp",
			[](void)->void { /*MessageBox(NULL, "sup2", "sup2", MB_OK);*/ },
			"Tune");

		//m_toolbar.SetVertical(false);
		//m_toolbar.ShowText();
		m_toolbar.Create();

		m_initialized = true;

		m_camera.LookAt(Eigen::Vector3f(0, 0, 0),
			Eigen::Vector3f(0.0f, 1.0f, 0.0f),
			Eigen::Vector3f(0.0f, 0.0f, 1.0f));

		m_camera.UpdateView();

		VertexDeclaration decl;
		decl.AddAttribute(VertexDeclaration::VDA_POSITION,
			VertexDeclaration::VDAT_FLOAT3);

		Mesh *triangle = m_model.AddMesh();
		triangle->Allocate(decl, 1);

		DirectX::XMFLOAT3 vertices[3] = {
			DirectX::XMFLOAT3(0, 0, 2.0f),
			DirectX::XMFLOAT3(0, 2.0f, 2.0f),
			DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f)
		};

		triangle->SetVertexAttribute(
			0,
			0,
			VertexDeclaration::VDA_POSITION,
			VertexDeclaration::VDAT_FLOAT3,
			&vertices[0]);

		triangle->SetVertexAttribute(
			0,
			1,
			VertexDeclaration::VDA_POSITION,
			VertexDeclaration::VDAT_FLOAT3,
			&vertices[1]);

		triangle->SetVertexAttribute(
			0,
			2,
			VertexDeclaration::VDA_POSITION,
			VertexDeclaration::VDAT_FLOAT3,
			&vertices[2]);

		m_vbuffer.Create(triangle);
		m_mvpBuffer.Create(sizeof(float) * 16, Eigen::Matrix4f(Eigen::Matrix4f::Identity()).data());

	}

	g_mainWindow.SetToolbar(&m_toolbar);
	g_mainWindow.ResizeViews();

	m_window.Show();
	m_toolbar.Show();


}

void ModelView::Deactivate(void)
{
	m_window.Hide();
	m_toolbar.Hide();
	g_mainWindow.SetToolbar(NULL);
}

void ModelView::SetPosition(const Eigen::Vector2i &position)
{
	m_window.SetPosition(position);
}

void ModelView::SetSize(const Eigen::Vector2i &size)
{

	View::SetSize(size);
	m_window.SetSize(size);
	m_toolbar.AutoSize();

	float ratio = (float) size.x() / (size.y() == 0 ? 1 : size.y());
	m_camera.SetClipAspectRatio(ratio);

	m_camera.UpdateProjection();

	g_mainWindow.Update();

}

void ModelView::SetBackgroundColor(const mye::core::ColorRGBA &rgba)
{
	m_bgColor = rgba;
}

void ModelView::Update(void)
{

}

void ModelView::Render(void)
{

	ID3D11DeviceContext *context = m_device.GetImmediateContext();

	context->ClearRenderTargetView(
		m_window.GetRenderTargetView(),
		m_bgColor.GetData());

	context->ClearDepthStencilView(
		m_window.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1,
		0);

	const Model::MeshList &meshList = m_model.GetMeshList();

	ID3D11Buffer *buffer = m_vbuffer.GetBuffer();

	UINT stride = sizeof(float) * 3;
	UINT offset = 0;

	m_mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);

	Eigen::Matrix4f mvp = m_transform * m_camera.GetViewProjectionMatrix();
	m_mvpBuffer.SetData(reinterpret_cast<const void*>(mvp.data()));

	m_vbuffer.Bind();

	/*context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/
	context->Draw(3, 0);

	m_window.GetSwapChain()->Present(1, 0);

}

mye::dx11::DX11Window& ModelView::GetWindow(void)
{
	return m_window;
}