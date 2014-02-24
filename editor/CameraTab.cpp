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

void SceneView::_CreateCameraTab(void)
{

	m_cameraTab.CreateChild(m_tabs.GetHandle());

	Checkbox *cameraCheckbox = new Checkbox;

	m_controls["CAMcameraCheckbox"] = cameraCheckbox;
	cameraCheckbox->Create(m_cameraTab,
		"Attach camera component",
		Vector2i(0, 0),
		Vector2i(240, 24));

	Edit *positionLabel = new Edit;

	positionLabel->Create(m_cameraTab,
		Vector2i(0, 28),
		Vector2i(90, 22));

	m_controls["CAMpositionLabel"] = positionLabel;
	positionLabel->SetText("Position:");
	positionLabel->SetReadOnly(true);
	positionLabel->SetBorder(false);

	Edit *positionXEdit = new Edit;

	m_controls["CAMpositionXEdit"] = positionXEdit;
	positionXEdit->Create(m_cameraTab,
		Vector2i(100, 26),
		Vector2i(50, 22));

	Edit *positionYEdit = new Edit;

	m_controls["CAMpositionYEdit"] = positionYEdit;
	positionYEdit->Create(m_cameraTab,
		Vector2i(154, 26),
		Vector2i(50, 22));

	Edit *positionZEdit = new Edit;

	m_controls["CAMpositionZEdit"] = positionZEdit;
	positionZEdit->Create(m_cameraTab,
		Vector2i(208, 26),
		Vector2i(50, 22));

	Edit *upLabel = new Edit;

	upLabel->Create(m_cameraTab,
		Vector2i(0, 54),
		Vector2i(90, 22));

	m_controls["CAMupLabel"] = upLabel;
	upLabel->SetText("Up vector:");
	upLabel->SetReadOnly(true);
	upLabel->SetBorder(false);

	Edit *upXEdit = new Edit;

	m_controls["CAMupXEdit"] = upXEdit;
	upXEdit->Create(m_cameraTab,
		Vector2i(100, 52),
		Vector2i(50, 22));

	Edit *upYEdit = new Edit;

	m_controls["CAMupYEdit"] = upYEdit;
	upYEdit->Create(m_cameraTab,
		Vector2i(154, 52),
		Vector2i(50, 22));

	Edit *upZEdit = new Edit;

	m_controls["CAMupZEdit"] = upZEdit;
	upZEdit->Create(m_cameraTab,
		Vector2i(208, 52),
		Vector2i(50, 22));

	Edit *lookAtLabel = new Edit;

	lookAtLabel->Create(m_cameraTab,
		Vector2i(0, 80),
		Vector2i(90, 22));

	m_controls["CAMupLabel"] = upLabel;
	lookAtLabel->SetText("Look at:");
	lookAtLabel->SetReadOnly(true);
	lookAtLabel->SetBorder(false);

	Edit *lookAtXEdit = new Edit;

	m_controls["CAMlookAtXEdit"] = lookAtXEdit;
	lookAtXEdit->Create(m_cameraTab,
		Vector2i(100, 78),
		Vector2i(50, 22));

	Edit *lookAtYEdit = new Edit;

	m_controls["CAMlookAtYEdit"] = lookAtYEdit;
	lookAtYEdit->Create(m_cameraTab,
		Vector2i(154, 78),
		Vector2i(50, 22));

	Edit *lookAtZEdit = new Edit;

	m_controls["CAMlookAtZEdit"] = lookAtZEdit;
	lookAtZEdit->Create(m_cameraTab,
		Vector2i(208, 78),
		Vector2i(50, 22));

	Checkbox *defaultCheckbox = new Checkbox;

	m_controls["CAMdefaultCheckbox"] = defaultCheckbox;
	defaultCheckbox->Create(m_cameraTab,
		"Use as default camera",
		Vector2i(0, 104),
		Vector2i(240, 24));

	Button *applyChangesButton = new Button;

	m_controls["CAMapplyChangesButton"] = applyChangesButton;
	applyChangesButton->Create(m_cameraTab,
		"Apply changes",
		[this] (void) -> void
	{

		OBJECT_SELECTED_NEEDED();

		Checkbox *cameraCheckbox = static_cast<Checkbox*>(m_controls["CAMcameraCheckbox"]);
		Checkbox *defaultCheckbox = static_cast<Checkbox*>(m_controls["CAMdefaultCheckbox"]);

		if (cameraCheckbox->IsChecked())
		{

			CameraComponent *camera = m_selected->GetCameraComponent();

			if (!camera)
			{

				m_selected->AddComponent(CameraComponent());
				camera = m_selected->GetCameraComponent();

			}

			camera->LookAt(
				Vector3f(
					atoi(static_cast<Edit*>(m_controls["CAMpositionXEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMpositionYEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMpositionZEdit"])->GetText().CString())),
				Vector3f(
					atoi(static_cast<Edit*>(m_controls["CAMupXEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMupYEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMupZEdit"])->GetText().CString())),
				Vector3f(
					atoi(static_cast<Edit*>(m_controls["CAMlookAtXEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMlookAtYEdit"])->GetText().CString()),
					atoi(static_cast<Edit*>(m_controls["CAMlookAtZEdit"])->GetText().CString())));

			if (defaultCheckbox->IsChecked())
			{
				g_scene.SetCamera(camera);
			}
			else if (g_scene.GetCamera() == camera)
			{
				g_scene.SetCamera(nullptr);
			}


		}

	},
		Vector2i(0, 130),
		Vector2i(158, 24));

	Button *discardChangesButton = new Button;

	m_controls["CAMdiscardChangesButton"] = discardChangesButton;
	discardChangesButton->Create(m_cameraTab,
		"Discard changes",
		[this] (void) -> void
	{
	},
		Vector2i(164, 130),
		Vector2i(96, 24));

	m_cameraTab.RegisterControl(cameraCheckbox);
	m_cameraTab.RegisterControl(applyChangesButton);
	m_cameraTab.RegisterControl(discardChangesButton);
	m_cameraTab.RegisterControl(defaultCheckbox);

}

void SceneView::_FillCameraTab(mye::core::GameObject *selectedObject)
{

	bool filled = false;

	Checkbox *checkbox = static_cast<Checkbox*>(m_controls["CAMcameraCheckbox"]);
	Checkbox *defaultCheckbox = static_cast<Checkbox*>(m_controls["CAMdefaultCheckbox"]);

	Edit *positionX = static_cast<Edit*>(m_controls["CAMpositionXEdit"]);
	Edit *positionY = static_cast<Edit*>(m_controls["CAMpositionYEdit"]);
	Edit *positionZ = static_cast<Edit*>(m_controls["CAMpositionZEdit"]);

	Edit *upX = static_cast<Edit*>(m_controls["CAMupXEdit"]);
	Edit *upY = static_cast<Edit*>(m_controls["CAMupYEdit"]);
	Edit *upZ = static_cast<Edit*>(m_controls["CAMupZEdit"]);

	Edit *lookAtX = static_cast<Edit*>(m_controls["CAMlookAtXEdit"]);
	Edit *lookAtY = static_cast<Edit*>(m_controls["CAMlookAtYEdit"]);
	Edit *lookAtZ = static_cast<Edit*>(m_controls["CAMlookAtZEdit"]);

	if (m_selected)
	{

		CameraComponent *camera = selectedObject->GetCameraComponent();

		if (camera)
		{

			checkbox->SetCheck(true);
			defaultCheckbox->SetCheck(g_scene.GetCamera() == camera);

			Vector3f position = camera->GetPosition();
			Vector3f up = camera->Up();
			Vector3f lookAt = position + camera->Forward();

			positionX->SetText(ToString(position.x()));
			positionY->SetText(ToString(position.y()));
			positionZ->SetText(ToString(position.z()));

			upX->SetText(ToString(up.x()));
			upY->SetText(ToString(up.y()));
			upZ->SetText(ToString(up.z()));

			lookAtX->SetText(ToString(lookAt.x()));
			lookAtY->SetText(ToString(lookAt.y()));
			lookAtZ->SetText(ToString(lookAt.z()));

			filled = true;

		}
	}

	if (!filled)
	{

		checkbox->SetCheck(false);
		defaultCheckbox->SetCheck(false);

		positionX->SetText(ToString(0.0f));
		positionY->SetText(ToString(0.0f));
		positionZ->SetText(ToString(0.0f));

		upX->SetText(ToString(0.0f));
		upY->SetText(ToString(1.0f));
		upZ->SetText(ToString(0.0f));

		lookAtX->SetText(ToString(0.0f));
		lookAtY->SetText(ToString(0.0f));
		lookAtZ->SetText(ToString(1.0f));

	}

}