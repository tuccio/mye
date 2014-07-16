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

ModelView::ModelView(void) :
	m_initialized(false),
	m_toolbar(g_mainWindow, true),
	m_bgColor(0.12f, 0.12f, 0.12f, 1.0f),
	m_vbuffer(nullptr, "", nullptr, *g_dx11graphics.GetDevice()),
	m_PlanetBuffer(nullptr, "", nullptr, *g_dx11graphics.GetDevice()),
	m_mvpBuffer(nullptr, "", nullptr, *g_dx11graphics.GetDevice()),
	m_transform(Transform::Identity())
{
	m_toolbar.SetIconSize(mye::math::Vector2i(24, 24));
}


ModelView::~ModelView(void)
{
	m_vbuffer.Destroy();
}

void ModelView::Activate(void)
{

	if (!m_initialized)
	{

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
			ofn.lpstrFileTitle  = nullptr;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn))
			{

				if (m_model)
				{
					m_model.reset();
				}

				Parameters params;

				params["normals"]   = "true";
				params["texcoords"] = "true";

				m_model = ResourceTypeManager::GetSingleton().
					CreateResource<Model>("Model", buffer, nullptr, params);

				m_model->Load();

				Model *model = m_model.get();

				m_vbuffer.Destroy();
				m_vbuffer.Create(model);

				auto minmax          = model->GetMinMaxVertices();
				AABBf box            = AABBf::FromMinMax(minmax.first, minmax.second);

				Vector3f halfExtents = box.GetHalfExtents();
				float scale          = 1.0f / (2.0f * Max(halfExtents.x(), halfExtents.y()));

				Vector3f center      = box.GetCenter();

				m_transform          = Transform::Identity();

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

		Vector2i size = g_renderWindow.GetSize();

		m_camera.SetProjection(75.0f,
			(float) size.x() / size.y(),
			0.1f,
			100.0f);

		m_mvpBuffer.Create(sizeof(float) * 16, Matrix4f(1.0f).Data());

// 		Mesh Planet(nullptr, "Planet", nullptr);
// 
// 		VertexDeclaration vd;
// 
// 		vd.AddAttribute(VertexDeclaration::VDA_POSITION,
// 			VertexDeclaration::VDAT_FLOAT3);
// 
// 		Planet.Allocate(vd, 2);
// 
// 		m_PlanetBuffer.Create(&Planet);

	}

	g_mainWindow.SetToolbar(&m_toolbar);
	g_mainWindow.ResizeViews();

	g_renderWindow.Show();
	m_toolbar.Show();


}

void ModelView::Deactivate(void)
{
	g_renderWindow.Hide();
	m_toolbar.Hide();
	g_mainWindow.SetToolbar(nullptr);
}

void ModelView::SetPosition(const mye::math::Vector2i &position)
{
	g_renderWindow.SetPosition(position);
}

void ModelView::SetSize(const mye::math::Vector2i &size)
{

	View::SetSize(size);
	g_renderWindow.SetSize(size);
	m_toolbar.AutoSize();

	float ratio = (float) size.x() / (size.y() == 0 ? 1 : size.y());
	m_camera.SetClipAspectRatio(ratio);

	m_camera.UpdateProjection();

	g_mainWindow.Update();

}

void ModelView::SetBackgroundColor(const mye::math::Vector4f &rgba)
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
					Vector3f(0.0f, - 0.025f * delta.y(), 0.0f) +
					Vector3f(0.025f * delta.x(), 0.0f, 0.0f));

			}

			
		}

		break;

	default:
		break;

	}

}

void ModelView::Render(void)
{

	ID3D11DeviceContext *context = g_dx11graphics.GetDevice()->GetImmediateContext();

	g_depthBuffer.Clear();
	g_swapChain.ClearBackBuffer(m_bgColor);

	m_mvpBuffer.Bind(PIPELINE_VERTEX_SHADER, 0);

	Matrix4f projection = m_camera.GetProjectionMatrix();
	Matrix4f view = m_camera.GetViewMatrix();

	Matrix4f mvp = m_camera.GetProjectionMatrix() *
		m_camera.GetViewMatrix() *
		m_transform.GetSRTMatrix();

	m_mvpBuffer.SetData(reinterpret_cast<const void*>(mvp.Data()));

	m_vbuffer.Bind();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(m_vbuffer.GetVerticesCount(), 0);

	g_swapChain->Present(1, 0);

}

mye::win::Window& ModelView::GetWindow(void)
{
	return g_renderWindow;
}

void ModelView::UpdateBuffer(void)
{

}