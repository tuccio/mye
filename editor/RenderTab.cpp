#include "SceneView.h"
#include "Globals.h"

#include <mye/win/Button.h>
#include <mye/win/Checkbox.h>
#include <mye/win/Edit.h>
#include <mye/win/Utils.h>

#include <mye/core/Model.h>
#include <mye/core/ResourceTypeManager.h>

#include <cstdlib>

#define OBJECT_SELECTED_NEEDED() if (!m_selected) { ShowErrorBox("No object selected", g_mainWindow.GetHandle()); return; }

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

		OBJECT_SELECTED_NEEDED()

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

	Edit *aabbCenterLabel = new Edit;

	aabbCenterLabel->Create(m_renderTab,
		Vector2i(0, 52),
		Vector2i(70, 22));

	m_controls["RNDaabbCenterLabel"] = aabbCenterLabel;
	aabbCenterLabel->SetText("AABB Center:");
	aabbCenterLabel->SetReadOnly(true);
	aabbCenterLabel->SetBorder(false);

	Edit *aabbCenterXEdit = new Edit;

	m_controls["RNDaabbCenterXEdit"] = aabbCenterXEdit;
	aabbCenterXEdit->Create(m_renderTab,
		Vector2i(100, 52),
		Vector2i(50, 22));

	Edit *aabbCenterYEdit = new Edit;

	m_controls["RNDaabbCenterYEdit"] = aabbCenterYEdit;
	aabbCenterYEdit->Create(m_renderTab,
		Vector2i(154, 52),
		Vector2i(50, 22));

	Edit *aabbCenterZEdit = new Edit;

	m_controls["RNDaabbCenterZEdit"] = aabbCenterZEdit;
	aabbCenterZEdit->Create(m_renderTab,
		Vector2i(208, 52),
		Vector2i(50, 22));

	Edit *aabbHalfExtentsLabel = new Edit;

	aabbHalfExtentsLabel->Create(m_renderTab,
		Vector2i(0, 78),
		Vector2i(96, 22));

	m_controls["RNDaabbHalfExtentsLabel"] = aabbHalfExtentsLabel;
	aabbHalfExtentsLabel->SetText("AABB Half Extents:");
	aabbHalfExtentsLabel->SetReadOnly(true);
	aabbHalfExtentsLabel->SetBorder(false);

	Edit *aabbHalfExtentsXEdit = new Edit;

	m_controls["RNDaabbHalfExtentsXEdit"] = aabbHalfExtentsXEdit;
	aabbHalfExtentsXEdit->Create(m_renderTab,
		Vector2i(100, 78),
		Vector2i(50, 22));

	Edit *aabbHalfExtentsYEdit = new Edit;

	m_controls["RNDaabbHalfExtentsYEdit"] = aabbHalfExtentsYEdit;
	aabbHalfExtentsYEdit->Create(m_renderTab,
		Vector2i(154, 78),
		Vector2i(50, 22));

	Edit *aabbHalfExtentsZEdit = new Edit;

	m_controls["RNDaabbHalfExtentsZEdit"] = aabbHalfExtentsZEdit;
	aabbHalfExtentsZEdit->Create(m_renderTab,
		Vector2i(208, 78),
		Vector2i(50, 22));

	Button *aabbAutoSizeButton = new Button;

	m_controls["RNDaabbAutoSizeButton"] = aabbAutoSizeButton;
	aabbAutoSizeButton->Create(m_renderTab,
		"Auto-Size AABB",
		[this] (void) -> void
	{

		RenderComponent *rc = m_selected->GetRenderComponent();

		if (rc)
		{

			AABBf aabb;

			ModelPointer model = rc->GetModel();

			if (model)
			{
				model->Load();
				auto minMax = model->GetMinMaxVertices();
				aabb = AABBf::FromMinMax(minMax.first, minMax.second);
			}
			else
			{
				aabb = AABBf::FromMinMax(Vector3f(0), Vector3f(0));
			}

			Vector3f aabbCenter = aabb.GetCenter();
			Vector3f aabbHalfExtents = aabb.GetHalfExtents();

			static_cast<Edit*>(m_controls["RNDaabbCenterXEdit"])->SetText(ToString(aabbCenter.x()));
			static_cast<Edit*>(m_controls["RNDaabbCenterYEdit"])->SetText(ToString(aabbCenter.y()));
			static_cast<Edit*>(m_controls["RNDaabbCenterZEdit"])->SetText(ToString(aabbCenter.z()));

			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsXEdit"])->SetText(ToString(aabbHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsYEdit"])->SetText(ToString(aabbHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsZEdit"])->SetText(ToString(aabbHalfExtents.z()));

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

		OBJECT_SELECTED_NEEDED();

		if (static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->IsChecked())
		{

			RenderComponent *rc = m_selected->GetRenderComponent();
			AABBf aabb;

			if (!rc)
			{

				g_scene.RemoveGameObject(m_selected->GetHandle());

				RenderComponent nRC;
				m_selected->AddComponent(nRC);
				rc = m_selected->GetRenderComponent();

				g_scene.AddGameObject(m_selected->GetHandle());

			}

			aabb = m_selected->GetAABB();

			Vector3f aabbCenter(
				atoi(static_cast<Edit*>(m_controls["RNDaabbCenterXEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDaabbCenterYEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDaabbCenterZEdit"])->GetText().CString()));

			Vector3f aabbHalfExtents(
				atoi(static_cast<Edit*>(m_controls["RNDaabbHalfExtentsXEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDaabbHalfExtentsYEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDaabbHalfExtentsZEdit"])->GetText().CString()));

			rc->SetBounds(AABBf::FromCenterHalfExtents(aabbCenter, aabbHalfExtents));
			g_scene.MoveGameObject(m_selected->GetHandle(), aabb);

			ModelPointer model = rc->GetModel();

			String modelPath = static_cast<Edit*>(m_controls["RNDmodelEdit"])->GetText();

			if (model && model->GetName() != modelPath ||
				!model && !modelPath.IsEmpty())
			{

				model = ResourceTypeManager::GetSingleton().
					CreateResource<Model>(
						"Model",
						modelPath, 
						nullptr);

				rc->SetModel(model);

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
	m_renderTab.RegisterControl(aabbAutoSizeButton);
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

			AABBf aabb               = renderComponent->GetBounds();
			Vector3f aabbCenter      = aabb.GetCenter();
			Vector3f aabbHalfExtents = aabb.GetHalfExtents();

			if (renderComponent->GetModel())
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText(renderComponent->GetModel()->GetName());
			}
			else
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");
			}

			static_cast<Edit*>(m_controls["RNDaabbCenterXEdit"])->SetText(ToString(aabbCenter.x()));
			static_cast<Edit*>(m_controls["RNDaabbCenterYEdit"])->SetText(ToString(aabbCenter.y()));
			static_cast<Edit*>(m_controls["RNDaabbCenterZEdit"])->SetText(ToString(aabbCenter.z()));

			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsXEdit"])->SetText(ToString(aabbHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsYEdit"])->SetText(ToString(aabbHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDaabbHalfExtentsZEdit"])->SetText(ToString(aabbHalfExtents.z()));

			renderComponentFilled = true;

		}

	}

	if (!renderComponentFilled)
	{

		static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->SetCheck(false);

		static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");

		static_cast<Edit*>(m_controls["RNDaabbCenterXEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDaabbCenterYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDaabbCenterZEdit"])->SetText("0");

		static_cast<Edit*>(m_controls["RNDaabbHalfExtentsXEdit"])->SetText("0");;
		static_cast<Edit*>(m_controls["RNDaabbHalfExtentsYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDaabbHalfExtentsZEdit"])->SetText("0");

	}

}