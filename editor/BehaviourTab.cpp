#include "SceneView.h"
#include "Globals.h"

#include <mye/win/Button.h>
#include <mye/win/Checkbox.h>
#include <mye/win/Edit.h>
#include <mye/win/ListView.h>
#include <mye/win/Utils.h>

#include <mye/core/Model.h>
#include <mye/core/ResourceTypeManager.h>

#include <cstdlib>

#define OBJECT_SELECTED_NEEDED() if (!m_selected) { ShowErrorBox("No object selected", g_mainWindow.GetHandle()); return; }

using namespace mye::dx11;
using namespace mye::win;
using namespace mye::math;
using namespace mye::core;

void SceneView::_CreateBehaviourTab(void)
{

	m_behaviourTab.CreateChild(m_tabs.GetHandle());

	Checkbox *behaviourCheckbox = new Checkbox;

	m_controls["BVRbehaviourCheckbox"] = behaviourCheckbox;
	behaviourCheckbox->Create(m_behaviourTab,
		"Attach behaviour component",
		Vector2i(0, 0),
		Vector2i(240, 24));

	Edit *entityLabel = new Edit;
	entityLabel->Create(m_behaviourTab,
		Vector2i(0, 26),
		Vector2i(80, 24));

	entityLabel->SetText("Entity name:");
	entityLabel->SetReadOnly(true);
	entityLabel->SetBorder(false);

	Edit *entityEdit = new Edit;
	m_controls["BVRentityEdit"] = entityEdit;

	entityEdit->Create(m_behaviourTab,
		Vector2i(80, 26),
		Vector2i(160, 24));

	Button *applyChangesButton = new Button;

	m_controls["BVRapplyChangesButton"] = applyChangesButton;
	applyChangesButton->Create(m_behaviourTab,
		"Apply changes",
		[this] (void) -> void
	{

		OBJECT_SELECTED_NEEDED();

		Checkbox *behaviourCheckbox = static_cast<Checkbox*>(m_controls["BVRbehaviourCheckbox"]);

		if (behaviourCheckbox->IsChecked())
		{

		}

	},
		Vector2i(0, 130),
		Vector2i(158, 24));

	Button *discardChangesButton = new Button;

	m_controls["BVRdiscardChangesButton"] = discardChangesButton;
	discardChangesButton->Create(m_behaviourTab,
		"Discard changes",
		[this] (void) -> void
	{
	},
		Vector2i(164, 130),
		Vector2i(96, 24));

	m_behaviourTab.RegisterControl(behaviourCheckbox);
	m_behaviourTab.RegisterControl(applyChangesButton);
	m_behaviourTab.RegisterControl(discardChangesButton);

}

void SceneView::_FillBehaviourTab(mye::core::GameObject *selectedObject)
{

	bool filled = false;

	Checkbox *checkbox = static_cast<Checkbox*>(m_controls["BVRbehaviourCheckbox"]);

	if (selectedObject)
	{

		
	}

	if (!filled)
	{

		checkbox->SetCheck(false);

	}

}