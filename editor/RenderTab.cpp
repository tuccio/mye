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

	Edit *AABBtCenterLabel = new Edit;

	AABBtCenterLabel->Create(m_renderTab,
		Vector2i(0, 52),
		Vector2i(70, 22));

	m_controls["RNDAABBtCenterLabel"] = AABBtCenterLabel;
	AABBtCenterLabel->SetText("AABBt Center:");
	AABBtCenterLabel->SetReadOnly(true);
	AABBtCenterLabel->SetBorder(false);

	Edit *AABBtCenterXEdit = new Edit;

	m_controls["RNDAABBtCenterXEdit"] = AABBtCenterXEdit;
	AABBtCenterXEdit->Create(m_renderTab,
		Vector2i(100, 52),
		Vector2i(50, 22));

	Edit *AABBtCenterYEdit = new Edit;

	m_controls["RNDAABBtCenterYEdit"] = AABBtCenterYEdit;
	AABBtCenterYEdit->Create(m_renderTab,
		Vector2i(154, 52),
		Vector2i(50, 22));

	Edit *AABBtCenterZEdit = new Edit;

	m_controls["RNDAABBtCenterZEdit"] = AABBtCenterZEdit;
	AABBtCenterZEdit->Create(m_renderTab,
		Vector2i(208, 52),
		Vector2i(50, 22));

	Edit *AABBtHalfExtentsLabel = new Edit;

	AABBtHalfExtentsLabel->Create(m_renderTab,
		Vector2i(0, 78),
		Vector2i(96, 22));

	m_controls["RNDAABBtHalfExtentsLabel"] = AABBtHalfExtentsLabel;
	AABBtHalfExtentsLabel->SetText("AABBt Half Extents:");
	AABBtHalfExtentsLabel->SetReadOnly(true);
	AABBtHalfExtentsLabel->SetBorder(false);

	Edit *AABBtHalfExtentsXEdit = new Edit;

	m_controls["RNDAABBtHalfExtentsXEdit"] = AABBtHalfExtentsXEdit;
	AABBtHalfExtentsXEdit->Create(m_renderTab,
		Vector2i(100, 78),
		Vector2i(50, 22));

	Edit *AABBtHalfExtentsYEdit = new Edit;

	m_controls["RNDAABBtHalfExtentsYEdit"] = AABBtHalfExtentsYEdit;
	AABBtHalfExtentsYEdit->Create(m_renderTab,
		Vector2i(154, 78),
		Vector2i(50, 22));

	Edit *AABBtHalfExtentsZEdit = new Edit;

	m_controls["RNDAABBtHalfExtentsZEdit"] = AABBtHalfExtentsZEdit;
	AABBtHalfExtentsZEdit->Create(m_renderTab,
		Vector2i(208, 78),
		Vector2i(50, 22));

	Button *AABBtAutoSizeButton = new Button;

	m_controls["RNDAABBtAutoSizeButton"] = AABBtAutoSizeButton;
	AABBtAutoSizeButton->Create(m_renderTab,
		"Auto-Size AABBt",
		[this] (void) -> void
	{

		RenderComponent *rc = m_selected->GetRenderComponent();

		if (rc)
		{

			AABBf AABBt;

			ModelPointer model = rc->GetModel();

			if (model)
			{
				model->Load();
				auto minMax = model->GetMinMaxVertices();
				AABBt = AABBf::FromMinMax(minMax.first, minMax.second);
			}
			else
			{
				AABBt = AABBf::FromMinMax(Vector3f(0), Vector3f(0));
			}

			Vector3f AABBtCenter = AABBt.GetCenter();
			Vector3f AABBtHalfExtents = AABBt.GetHalfExtents();

			static_cast<Edit*>(m_controls["RNDAABBtCenterXEdit"])->SetText(ToString(AABBtCenter.x()));
			static_cast<Edit*>(m_controls["RNDAABBtCenterYEdit"])->SetText(ToString(AABBtCenter.y()));
			static_cast<Edit*>(m_controls["RNDAABBtCenterZEdit"])->SetText(ToString(AABBtCenter.z()));

			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsXEdit"])->SetText(ToString(AABBtHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsYEdit"])->SetText(ToString(AABBtHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsZEdit"])->SetText(ToString(AABBtHalfExtents.z()));

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
			AABBf AABBt;

			if (!rc)
			{

				g_scene.RemoveGameObject(m_selected->GetHandle());

				RenderComponent nRC;
				m_selected->AddComponent(nRC);
				rc = m_selected->GetRenderComponent();

				g_scene.AddGameObject(m_selected->GetHandle());

			}

			AABBt = m_selected->GetAABB();

			Vector3f AABBtCenter(
				atoi(static_cast<Edit*>(m_controls["RNDAABBtCenterXEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDAABBtCenterYEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDAABBtCenterZEdit"])->GetText().CString()));

			Vector3f AABBtHalfExtents(
				atoi(static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsXEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsYEdit"])->GetText().CString()),
				atoi(static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsZEdit"])->GetText().CString()));

			rc->SetBounds(AABBf::FromCenterHalfExtents(AABBtCenter, AABBtHalfExtents));
			g_scene.MoveGameObject(m_selected->GetHandle(), AABBt);

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
	m_renderTab.RegisterControl(AABBtAutoSizeButton);
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

			AABBf AABBt               = renderComponent->GetBounds();
			Vector3f AABBtCenter      = AABBt.GetCenter();
			Vector3f AABBtHalfExtents = AABBt.GetHalfExtents();

			if (renderComponent->GetModel())
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText(renderComponent->GetModel()->GetName());
			}
			else
			{
				static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");
			}

			static_cast<Edit*>(m_controls["RNDAABBtCenterXEdit"])->SetText(ToString(AABBtCenter.x()));
			static_cast<Edit*>(m_controls["RNDAABBtCenterYEdit"])->SetText(ToString(AABBtCenter.y()));
			static_cast<Edit*>(m_controls["RNDAABBtCenterZEdit"])->SetText(ToString(AABBtCenter.z()));

			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsXEdit"])->SetText(ToString(AABBtHalfExtents.x()));
			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsYEdit"])->SetText(ToString(AABBtHalfExtents.y()));
			static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsZEdit"])->SetText(ToString(AABBtHalfExtents.z()));

			renderComponentFilled = true;

		}

	}

	if (!renderComponentFilled)
	{

		static_cast<Checkbox*>(m_controls["RNDrenderCheckbox"])->SetCheck(false);

		static_cast<Edit*>(m_controls["RNDmodelEdit"])->SetText("");

		static_cast<Edit*>(m_controls["RNDAABBtCenterXEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBtCenterYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBtCenterZEdit"])->SetText("0");

		static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsXEdit"])->SetText("0");;
		static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsYEdit"])->SetText("0");
		static_cast<Edit*>(m_controls["RNDAABBtHalfExtentsZEdit"])->SetText("0");

	}

}