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

#define OBJECT_SELECTED_NEEDED() if (!m_selected) { ShowErrorBox("No object selected"); return; }

using namespace mye::dx11;
using namespace mye::win;
using namespace mye::math;
using namespace mye::core;

void SceneView::_CreatePropertiesTab(void)
{

	m_propertiesTab.CreateChild(m_tabs.GetHandle());

	Edit *nameLabel = new Edit;
	m_controls["PRPnameLabel"] = nameLabel;

	nameLabel->Create(m_propertiesTab,
		Vector2i(0, 2),
		Vector2i(34, 22));

	nameLabel->SetText("Name:");
	nameLabel->SetReadOnly(true);
	nameLabel->SetBorder(false);

	Edit *nameEdit = new Edit;
	m_controls["PRPnameEdit"] = nameEdit;

	nameEdit->Create(m_propertiesTab,
		Vector2i(42, 0),
		Vector2i(220, 22));

	ListView *propList = new ListView;

	m_controls["PRPpropList"] = propList;

	propList->Create(m_propertiesTab,
		Vector2i(0, 26),
		Vector2i(260, 340));

	LV_COLUMN nameCol;
	nameCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	nameCol.pszText = "Name";
	nameCol.cx = 75;

	ListView_InsertColumn(propList->GetHandle(), 0, &nameCol);

	LV_COLUMN typeCol;

	typeCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	typeCol.pszText = "Type";
	typeCol.cx = 75;

	ListView_InsertColumn(propList->GetHandle(), 1, &typeCol);

	LV_COLUMN valueCol;

	valueCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	valueCol.pszText = "Value";
	valueCol.cx = 300;

	ListView_InsertColumn(propList->GetHandle(), 2, &valueCol);

	Button *applyButton = new Button;

	m_controls["PRPapplyButton"] = applyButton;

	applyButton->Create(m_propertiesTab,
		"Apply",
		[this, propList] (void) -> void
		{

			OBJECT_SELECTED_NEEDED();

			String objectName = static_cast<Edit*>(m_controls["PRPnameEdit"])->GetText();

			g_gameObjectsModule.Rename(
				m_selected->GetHandle(),
				objectName);

			ListView *gameObjectsList = static_cast<ListView*>(m_controls["GOTGameObjectsList"]);

			LV_FINDINFO findInfo;

			ZeroMemory(&findInfo, sizeof(LV_FINDINFO));

			findInfo.flags = LVFI_PARAM;
			findInfo.lParam = (LPARAM) m_selected;

			int i = ListView_FindItem(
				gameObjectsList->GetHandle(),
				-1,
				&findInfo);

			if (i >= 0)
			{

				LV_ITEM item;

				ZeroMemory(&item, sizeof(LV_ITEM));
				item.iItem = i;
				item.cchTextMax = 64;
				item.mask = LVIF_TEXT;
				item.pszText = (objectName.IsEmpty() ? "<Anon>" : objectName.CString());

				ListView_SetItem(gameObjectsList->GetHandle(), &item);

			}

		},
		Vector2i(0, 370),
		Vector2i(260, 24));

	m_propertiesTab.RegisterControl(applyButton);

}

void SceneView::_FillPropertiesTab(mye::core::GameObject *selectedObject)
{

	if (selectedObject)
	{

		static_cast<Edit*>(m_controls["PRPnameEdit"])->SetText(selectedObject->GetName());

		ListView *propList = static_cast<ListView*>(m_controls["PRPpropList"]);

		ListView_DeleteAllItems(propList->GetHandle());

		int i = 0;

		for (auto it : *selectedObject)
		{

			LV_ITEM item;

			ZeroMemory(&item, sizeof(LV_ITEM));
			item.iItem = i;
			item.cchTextMax = 64;
			item.mask = LVIF_TEXT;
			item.pszText = (LPSTR) it.first.CString();
			item.iSubItem = 0;

			ListView_InsertItem(propList->GetHandle(), &item);

			String componentType;
			String componentValue(255);

			switch (it.second->GetComponentType())
			{

			case ComponentTypes::TRANSFORM:
				componentType = "Transform Component";
				{
					TransformComponent *tc = static_cast<TransformComponent*>(it.second);
					componentValue = "{ position: " + ToString(tc->GetPosition()) +
						", orientation: " + ToString(tc->GetOrientation()) +
						", scale: " + ToString(tc->GetScale()) + " }";
				}
				
				break;

			}

			ListView_SetItemText(propList->GetHandle(), i, 1, (LPSTR) componentType.CString());
			ListView_SetItemText(propList->GetHandle(), i, 2, (LPSTR) componentValue.CString());

			i++;

		}

	}

}