#include "SceneView.h"
#include "Globals.h"

#include <mye/win/Button.h>
#include <mye/win/Checkbox.h>
#include <mye/win/Edit.h>
#include <mye/win/Utils.h>

#include <mye/core/Model.h>
#include <mye/core/ResourceTypeManager.h>

#include <cstdlib>

#define __ASSERT_OBJECT_SELECTED() if (!m_selected) { ShowErrorBox("No object selected", g_mainWindow.GetHandle()); return; }

using namespace mye::dx11;
using namespace mye::win;
using namespace mye::math;
using namespace mye::core;

void SceneView::_CreateRenderTab(void)
{

	m_renderTab.CreateChild(m_tabs.GetHandle());

	Edit *modelEdit = new Edit;
	m_controls["RNDmodelEdit"] = modelEdit;

	modelEdit->Create(m_renderTab,
		Vector2i(0, 27),
		Vector2i(178, 22));

	modelEdit->SetReadOnly(true);

	Button *browseModelButton = new Button;
	m_controls["RNDbrowseModelButton"] = browseModelButton;

	browseModelButton->Create(
		m_renderTab,
		"Browse...",
		[this] (void) -> void
	{

		__ASSERT_OBJECT_SELECTED()

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
			static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText(buffer);
		}

	},
		Vector2i(180, 26),
		Vector2i(80, 24));

	Checkbox *renderCheckbox = new Checkbox;

	m_controls["RNDrenderCheckbox"] = renderCheckbox;
	renderCheckbox->Create(m_renderTab,
		"Attach render component",
		Vector2i(0, 0),
		Vector2i(240, 24));

	Edit *AABBCenterLabel = new Edit;

	AABBCenterLabel->Create(m_renderTab,
		Vector2i(0, 52),
		Vector2i(70, 22));

	m_controls["RNDAABBCenterLabel"] = AABBCenterLabel;
	AABBCenterLabel->SetText("AABB Center:");
	AABBCenterLabel->SetReadOnly(true);
	AABBCenterLabel->SetBorder(false);

	Edit *AABBCenterXEdit = new Edit;

	m_controls["RNDAABBCenterXEdit"] = AABBCenterXEdit;
	AABBCenterXEdit->Create(m_renderTab,
		Vector2i(100, 52),
		Vector2i(50, 22));

	Edit *AABBCenterYEdit = new Edit;

	m_controls["RNDAABBCenterYEdit"] = AABBCenterYEdit;
	AABBCenterYEdit->Create(m_renderTab,
		Vector2i(154, 52),
		Vector2i(50, 22));

	Edit *AABBCenterZEdit = new Edit;

	m_controls["RNDAABBCenterZEdit"] = AABBCenterZEdit;
	AABBCenterZEdit->Create(m_renderTab,
		Vector2i(208, 52),
		Vector2i(50, 22));

	Edit *AABBHalfExtentsLabel = new Edit;

	AABBHalfExtentsLabel->Create(m_renderTab,
		Vector2i(0, 78),
		Vector2i(96, 22));

	m_controls["RNDAABBHalfExtentsLabel"] = AABBHalfExtentsLabel;
	AABBHalfExtentsLabel->SetText("AABB Half Extents:");
	AABBHalfExtentsLabel->SetReadOnly(true);
	AABBHalfExtentsLabel->SetBorder(false);

	Edit *AABBHalfExtentsXEdit = new Edit;

	m_controls["RNDAABBHalfExtentsXEdit"] = AABBHalfExtentsXEdit;
	AABBHalfExtentsXEdit->Create(m_renderTab,
		Vector2i(100, 78),
		Vector2i(50, 22));

	Edit *AABBHalfExtentsYEdit = new Edit;

	m_controls["RNDAABBHalfExtentsYEdit"] = AABBHalfExtentsYEdit;
	AABBHalfExtentsYEdit->Create(m_renderTab,
		Vector2i(154, 78),
		Vector2i(50, 22));

	Edit *AABBHalfExtentsZEdit = new Edit;

	m_controls["RNDAABBHalfExtentsZEdit"] = AABBHalfExtentsZEdit;
	AABBHalfExtentsZEdit->Create(m_renderTab,
		Vector2i(208, 78),
		Vector2i(50, 22));

	Button *AABBAutoSizeButton = new Button;

	m_controls["RNDAABBAutoSizeButton"] = AABBAutoSizeButton;
	AABBAutoSizeButton->Create(m_renderTab,
		"Auto-Size AABB",
		[this] (void) -> void
	{

		RenderComponent *rc = m_selected->GetRenderComponent();

		if (rc)
		{

			AABBf AABB;

			MeshPointer mesh = rc->GetMesh();

			if (mesh && mesh->Load())
			{
				auto minMax = mesh->GetMinMaxVertices();
				AABB = AABBf::FromMinMax(minMax.first, minMax.second);
			}
			else
			{
				AABB = AABBf::FromMinMax(Vector3f(0), Vector3f(0));
			}

			Vector3f AABBCenter = AABB.GetCenter();
			Vector3f AABBHalfExtents = AABB.GetHalfExtents();

			static_cast<Edit*>(m_controls["RNDAABBCenterXEdit"])->SetText(ToString(AABBCenter.x()));
			static_cast<Edit*>(m_controls["RNDAABBCenterYEdit"])->SetText(ToString(AABBCenter.y()));
			static_cast<Edit*>(m_controls["RNDAABBCenterZEdit"])->SetText(ToString(AABBCenter.z()));

			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsXEdit"])->SetText(ToString(AABBHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsYEdit"])->SetText(ToString(AABBHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsZEdit"])->SetText(ToString(AABBHalfExtents.z()));

		}

	},
		Vector2i(100, 104),
		Vector2i(160, 24));

	Button *applyChangesButton = new Button;

	m_controls["RNDapplyChangesButton"] = applyChangesButton;
	applyChangesButton->Create(m_renderTab,
		"Apply changes",
		[this] (void) -> void
	{

		__ASSERT_OBJECT_SELECTED();

		if (static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->IsChecked())
		{

			RenderComponent *rc = m_selected->GetRenderComponent();
			AABB box;

			if (!rc)
			{

				g_scene.RemoveGameObject(m_selected->GetHandle());

				RenderComponent nRC;
				m_selected->AddComponent(nRC);
				rc = m_selected->GetRenderComponent();

				g_scene.AddGameObject(m_selected->GetHandle());

			}

			box = m_selected->GetAABB();

			Vector3 boxCenter(
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBCenterXEdit"])->GetText()),
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBCenterYEdit"])->GetText()),
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBCenterZEdit"])->GetText()));

			Vector3 boxHalfExtents(
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBHalfExtentsXEdit"])->GetText()),
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBHalfExtentsYEdit"])->GetText()),
				ParseType<mye::math::Real>(static_cast<Edit*>(m_controls["RNDAABBHalfExtentsZEdit"])->GetText()));

			rc->SetBounds(AABB::FromCenterHalfExtents(boxCenter, boxHalfExtents));
			g_scene.MoveGameObject(m_selected->GetHandle(), box);

			MeshPointer mesh = rc->GetMesh();

			String modelPath = static_cast<Edit*>(m_controls["RNDmodelEdit"])->GetText();

			if (mesh && mesh->GetName() != modelPath ||
				!mesh && !modelPath.IsEmpty())
			{

				mesh = ResourceTypeManager::GetSingleton().
					CreateResource<Mesh>(
						"Mesh",
						modelPath, 
						nullptr);

				rc->SetMesh(mesh);

			}

		}
		else
		{

			RenderComponent *rc = m_selected->GetRenderComponent();

			if (rc)
			{
				g_scene.RemoveGameObject(m_selected->GetHandle());
				m_selected->RemoveComponent("render");
				g_scene.AddGameObject(m_selected->GetHandle());
			}

		}

	},
		Vector2i(0, 134),
		Vector2i(158, 24));

	Button *discardChangesButton = new Button;

	m_controls["RNDdiscardChangesButton"] = discardChangesButton;
	discardChangesButton->Create(m_renderTab,
		"Discard changes",
		[this] (void) -> void
	{
	},
		Vector2i(164, 134),
		Vector2i(96, 24));

	m_renderTab.RegisterControl(modelEdit);
	m_renderTab.RegisterControl(renderCheckbox);
	m_renderTab.RegisterControl(browseModelButton);
	m_renderTab.RegisterControl(AABBAutoSizeButton);
	m_renderTab.RegisterControl(applyChangesButton);
	m_renderTab.RegisterControl(discardChangesButton);

}

void SceneView::_FillRenderTab(mye::core::GameObject *selectedObject)
{

	bool renderComponentFilled = false;

	if (selectedObject)
	{

		RenderComponent *renderComponent = selectedObject->GetRenderComponent();

		if (renderComponent)
		{

			static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->SetCheck(true);

			AABBf AABB               = renderComponent->GetBounds();
			Vector3f AABBCenter      = AABB.GetCenter();
			Vector3f AABBHalfExtents = AABB.GetHalfExtents();

			if (renderComponent->GetMesh())
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText(renderComponent->GetMesh()->GetName());
			}
			else
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");
			}

			static_cast<Edit*>(m_controls["RNDAABBCenterXEdit"])->SetText(ToString(AABBCenter.x()));
			static_cast<Edit*>(m_controls["RNDAABBCenterYEdit"])->SetText(ToString(AABBCenter.y()));
			static_cast<Edit*>(m_controls["RNDAABBCenterZEdit"])->SetText(ToString(AABBCenter.z()));

			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsXEdit"])->SetText(ToString(AABBHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsYEdit"])->SetText(ToString(AABBHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDAABBHalfExtentsZEdit"])->SetText(ToString(AABBHalfExtents.z()));

			renderComponentFilled = true;

		}

	}

	if (!renderComponentFilled)
	{

		static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->SetCheck(false);

		static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");

		static_cast<Edit*>(m_controls["RNDAABBCenterXEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBCenterYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBCenterZEdit"])->SetText("0");

		static_cast<Edit*>(m_controls["RNDAABBHalfExtentsXEdit"])->SetText("0");;
		static_cast<Edit*>(m_controls["RNDAABBHalfExtentsYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBHalfExtentsZEdit"])->SetText("0");

	}

}