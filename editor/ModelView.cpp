#include "ModelView.h"
#include "Globals.h"

#include <mye/core/ManualLambdaLoader.h>
#include <mye/core/ResourceTypeManager.h>

#include <mye/d3d11/DX11VertexShader.h>
#include <mye/d3d11/DX11Utils.h>

#include <mye/win/Utils.h>

using namespace mye::core;
using namespace mye::dx11;
using namespace mye::math;
using namespace mye::win;

#define MVIEW_OPEN 0x0
#define MVIEW_TUNE 0x1

ModelView::ModelView(mye::dx11::DX11Device &device) :
	m_window(device),
	m_device(device),
	m_initialized(false),
	m_toolbar(g_mainWindow, true),
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_vbuffer(NULL, "", NULL, device),
	m_planeBuffer(NULL, "", NULL, device),
	m_mvpBuffer(NULL, "", NULL, device),
	m_transform(Transformf::Identity())
{
	m_toolbar.SetIconSize(mye::math::Vector2i(24, 24));
	m_window.SetMSAA(DX11Window::MSAA_4x);
}


ModelView::~ModelView(void)
{
	m_vbuffer.Destroy();
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
			ofn.lpstrFilter     = "Wavefront OBJ\0*.obj\0\03DS Max\0*.3ds;*.max\0\0";
			ofn.nFilterIndex    = 1;
			ofn.lpstrFileTitle  = NULL;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn))
			{

				if (m_model)
				{
					m_model->Unload();
					m_model.Release();
				}

				Resource::ParametersList params;

				params["normals"] = "false";
				params["texcoords"] = "false";

				m_model = ResourceTypeManager::GetSingleton().
					CreateResource("Model", buffer, NULL, &params);

				m_model->Load();

				Model *model = m_model.Cast<Model>();

				m_vbuffer.Destroy();
				m_vbuffer.Create(model);

				auto minmax = model->GetMinMaxVertices();
				AABB aabb(minmax.first, minmax.second);

				Vector3f halfExtents = aabb.GetHalfExtents();
				float scale = 1.0f / (2.0f * Max(halfExtents.x(), halfExtents.y()));

				Vector3f center = aabb.GetCenter();

				m_transform = Transformf::Identity();

				m_transform.SetScale(Vector3f(scale));
				m_transform.SetPosition(-center);
				m_transform.SetOrientation(Quaternionf(1, 0, 0, 0));

			}

		},
			"Open");

		m_toolbar.AddButton("icons/gear.bmp",
			[this](void)->void
		{

			m_inputMode = MODELVIEW_INPUT_TRANSFORM;

		},
			"Tune");

		m_toolbar.Create();

		m_initialized = true;

		m_camera.LookAt(
			Vector3f(0.0f, 0.0f, -2.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 0.0f));

		Vector2i size = m_window.GetSize();

		m_camera.SetFrustum(75.0f,
			(float) size.x() / size.y(),
			0.1f,
			100.0f);

		m_mvpBuffer.Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

// 		Mesh plane(NULL, "plane", NULL);
// 
// 		VertexDeclaration vd;
// 
// 		vd.AddAttribute(VertexDeclaration::VDA_POSITION,
// 			VertexDeclaration::VDAT_FLOAT3);
// 
// 		plane.Allocate(vd, 2);
// 
// 		m_planeBuffer.Create(&plane);

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

void ModelView::SetPosition(const mye::math::Vector2i &position)
{
	m_window.SetPosition(position);
}

void ModelView::SetSize(const mye::math::Vector2i &size)
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

	switch (m_inputMode)
	{

	case MODELVIEW_INPUT_TRANSFORM:

		{

			const Mouse *mouse = g_input.GetMouse();
			const Keyboard *keyboard = g_input.GetKeyboard();

			if (mouse->GetWheelDelta() != 0)
			{

				Vector3f scale = m_transform.GetScale();
				
				if (mouse->GetWheelDelta() > 0)
				{
					m_transform.SetScale(1.075f * scale);
				}
				else
				{
					m_transform.SetScale(0.925f * scale);
				}

			}

			if (mouse->IsPressed(MYE_VK_MOUSE_LEFT))
			{

				Vector2f delta = mouse->GetDelta();

// 				Matrix4f matrix = (m_camera.GetViewMatrix() *
// 					RotationMatrix4(m_transform.GetOrientation()));

// 				Quaternionf yAxisRotation(
// 					matrix * Vector3f(0, 1, 0),
// 					- delta.x() * 0.15f);
// 
// 				Quaternionf xAxisRotation(
// 					matrix * Vector3f(1, 0, 0),
// 					- delta.y() * 0.15);

				Quaternionf yAxisRotation(
					Vector3f(0, 1, 0),
					- delta.x() * 0.15f);

				Quaternionf xAxisRotation(
					Vector3f(1, 0, 0),
					- delta.y() * 0.15f);

				m_transform.SetOrientation(
					yAxisRotation *
					xAxisRotation *
					m_transform.GetOrientation());

			}
			
			if (mouse->IsPressed(MYE_VK_MOUSE_MIDDLE))
			{

				Vector2f delta = mouse->GetDelta();

// 				Matrix4f matrix = (m_camera.GetViewMatrix() *
// 					RotationMatrix4(m_transform.GetOrientation()));
// 
// 				Vector3f yAxisTranslation =
// 					matrix * (Vector3f(0, -0.025f, 0) * delta.y());
// 				
// 				Vector3f xAxisTranslation =
// 					matrix * (Vector3f(0.025f, 0, 0) * delta.x());

				m_transform.SetPosition(
					m_transform.GetPosition() +
					Vector3f(0, - 0.025 * delta.y(), 0) +
					Vector3f(0.025 * delta.x(), 0, 0));

			}

			
		}

		break;

	default:
			break;

	}

}

void ModelView::Render(void)
{

	ID3D11DeviceContext *context = m_device.GetImmediateContext();

	context->ClearRenderTargetView(
		m_window.GetRenderTargetView(),
		m_bgColor.Data());

	context->ClearDepthStencilView(
		m_window.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1,
		0);

	m_mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);

	Matrix4f projection = m_camera.GetProjectionMatrix();
	Matrix4f view = m_camera.GetViewMatrix();

	Matrix4f mvp = m_camera.GetProjectionMatrix() *
		m_camera.GetViewMatrix() *
		m_transform.GetTRSMatrix();

	m_mvpBuffer.SetData(reinterpret_cast<const void*>(mvp.Data()));

	m_vbuffer.Bind();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(m_vbuffer.GetVerticesCount(), 0);

	m_window.GetSwapChain()->Present(1, 0);

}

mye::dx11::DX11Window& ModelView::GetWindow(void)
{
	return m_window;
}

void ModelView::UpdateBuffer(void)
{

}