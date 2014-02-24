#include "Game.h"

#include "Logger.h"
#include "Stream.h"

#include "ResourceTypeManager.h"

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

#include <rapidxml_utils.hpp>

using namespace mye::core;
using namespace mye::math;

Game::Game(InputModule *input,
		   GameObjectsModule *gameobjects,
		   SceneModule *scene,
		   GraphicsModule *graphics,
		   AudioModule *audio,
		   ScriptModule *script) :
m_input(input),
	m_gameobjects(gameobjects),
	m_scene(scene),
	m_graphics(graphics),
	m_audio(audio),
	m_script(script)
{

}

Game::~Game(void)
{
}

bool Game::Init(void)
{

#define __TRY_INIT(x) if (!(x)->Init()) return false;

	__TRY_INIT(m_gameobjects)
	__TRY_INIT(m_scene)
	__TRY_INIT(m_graphics)
	__TRY_INIT(m_audio)
	__TRY_INIT(m_input)
	__TRY_INIT(m_script)

#undef __TRY_INIT

	return true;

}

void Game::Run(void)
{
	m_timer.Start();
}

void Game::Quit(void)
{
	m_timer.Stop();
	exit(0);
}

InputModule* Game::GetInputModule(void)
{
	return m_input;
}

GameObjectsModule* Game::GetGameObjectsModule(void)
{
	return m_gameobjects;
}

SceneModule* Game::GetSceneModule(void)
{
	return m_scene;
}

GraphicsModule* Game::GetGraphicsModule(void)
{
	return m_graphics;
}

AudioModule* Game::GetAudioModule(void)
{
	return m_audio;
}

ScriptModule* Game::GetScriptModule(void)
{
	return m_script;
}

void Game::RuntimeError(const String &error)
{
	mye::core::Logger::LogErrorOptional(error);
}

void Game::ExportScene(const String &path)
{

	FileOutputStream f(path);

	if (f)
	{

		rapidxml::xml_document<> document;

		rapidxml::xml_node<> *scene = document.allocate_node(
			rapidxml::node_element,
			"scene");

		document.append_node(scene);

		CameraComponent *activeCamera = static_cast<CameraComponent*>(m_scene->GetCamera());

		SceneModule::ObjectsList list = m_scene->GetObjects();

		for (GameObject *o : list)
		{

			rapidxml::xml_node<> *gameObjectNode = document.allocate_node(
				rapidxml::node_element,
				"gameobject");

			scene->append_node(gameObjectNode);

			gameObjectNode->append_attribute(document.allocate_attribute("name", o->GetName().CString()));

			for (auto it : *o)
			{

				switch (it.second->GetComponentType())
				{

				case ComponentTypes::TRANSFORM:

					{

						TransformComponent *tc = static_cast<TransformComponent*>(it.second);

						rapidxml::xml_node<> *transformNode = document.allocate_node(
							rapidxml::node_element,
							"transform");

						gameObjectNode->append_node(transformNode);

						String position = (ToString(tc->GetPosition().x()) +
							" " +
							ToString(tc->GetPosition().y()) +
							" " +
							ToString(tc->GetPosition().z()));

						String scale = (ToString(tc->GetScale().x()) +
							" " +
							ToString(tc->GetScale().y()) +
							" " +
							ToString(tc->GetScale().z()));

						String orientation = (ToString(tc->GetOrientation().w()) +
							" " +
							ToString(tc->GetOrientation().x()) +
							" " +
							ToString(tc->GetOrientation().y()) +
							" " +
							ToString(tc->GetOrientation().z()));

						rapidxml::xml_node<> *positionNode = document.allocate_node(
							rapidxml::node_element,
							"position",
							document.allocate_string(position.CString()));

						rapidxml::xml_node<> *scaleNode = document.allocate_node(
							rapidxml::node_element,
							"scale",
							document.allocate_string(scale.CString()));

						rapidxml::xml_node<> *orientationNode = document.allocate_node(
							rapidxml::node_element,
							"orientation",
							document.allocate_string(orientation.CString()));

						transformNode->append_node(positionNode);
						transformNode->append_node(scaleNode);
						transformNode->append_node(orientationNode);

					}

					break;

				case ComponentTypes::CAMERA:

					{

						CameraComponent *cc = static_cast<CameraComponent*>(it.second);

						rapidxml::xml_node<> *cameraNode = document.allocate_node(
							rapidxml::node_element,
							"camera");

						gameObjectNode->append_node(cameraNode);

						String position = (ToString(cc->GetPosition().x()) +
							" " +
							ToString(cc->GetPosition().y()) +
							" " +
							ToString(cc->GetPosition().z()));

						String orientation = (ToString(cc->GetOrientation().w()) +
							" " +
							ToString(cc->GetOrientation().x()) +
							" " +
							ToString(cc->GetOrientation().y()) +
							" " +
							ToString(cc->GetOrientation().z()));

						rapidxml::xml_node<> *positionNode = document.allocate_node(
							rapidxml::node_element,
							"position",
							document.allocate_string(position.CString()));

						rapidxml::xml_node<> *orientationNode = document.allocate_node(
							rapidxml::node_element,
							"orientation",
							document.allocate_string(orientation.CString()));

						rapidxml::xml_node<> *fovYNode = document.allocate_node(
							rapidxml::node_element,
							"fovy",
							document.allocate_string(ToString(cc->GetFovY()).CString()));

						rapidxml::xml_node<> *znearNode = document.allocate_node(
							rapidxml::node_element,
							"znear",
							document.allocate_string(ToString(cc->GetNearClipDistance()).CString()));

						rapidxml::xml_node<> *zfarNode = document.allocate_node(
							rapidxml::node_element,
							"zfar",
							document.allocate_string(ToString(cc->GetFarClipDistance()).CString()));

						if (cc == activeCamera)
						{
							cameraNode->append_attribute(document.allocate_attribute("active", "true"));
						}

						cameraNode->append_node(positionNode);
						cameraNode->append_node(orientationNode);
						cameraNode->append_node(fovYNode);
						cameraNode->append_node(znearNode);
						cameraNode->append_node(zfarNode);

					}

					break;

				case ComponentTypes::RENDER:

					{

						RenderComponent *rc = static_cast<RenderComponent*>(it.second);

						rapidxml::xml_node<> *renderNode = document.allocate_node(
							rapidxml::node_element,
							"render");

						gameObjectNode->append_node(renderNode);

						String aabbMin = (ToString(rc->GetBounds().GetMinimum().x()) +
							" " +
							ToString(rc->GetBounds().GetMinimum().y()) +
							" " +
							ToString(rc->GetBounds().GetMinimum().z()));

						String aabbMax = (ToString(rc->GetBounds().GetMaximum().x()) +
							" " +
							ToString(rc->GetBounds().GetMaximum().y()) +
							" " +
							ToString(rc->GetBounds().GetMaximum().z()));

						rapidxml::xml_node<> *aabb = document.allocate_node(
							rapidxml::node_element,
							"aabb");

						rapidxml::xml_node<> *model = document.allocate_node(
							rapidxml::node_element,
							"model");

						model->append_attribute(document.allocate_attribute(
							"name",
							document.allocate_string(rc->GetModel().get()->GetName().CString())));

						aabb->append_node(document.allocate_node(
							rapidxml::node_element,
							"min",
							document.allocate_string(aabbMin.CString())));

						aabb->append_node(document.allocate_node(
							rapidxml::node_element,
							"max",
							document.allocate_string(aabbMax.CString())));

						renderNode->append_node(aabb);
						renderNode->append_node(model);

					}

					break;

				}

			}


		}

		std::string xml_as_string;
		rapidxml::print(std::back_inserter(xml_as_string), document);

		f << xml_as_string.c_str();

		f.Close();

	}

	

}

void Game::ImportScene(const String &file, std::list<GameObject*> *allocatedObjects)
{

	rapidxml::file<> xmlFile(file.CString());

	rapidxml::xml_document<> document;
	document.parse<0>(xmlFile.data());

	rapidxml::xml_node<> *scene = document.first_node("scene");

	if (scene)
	{

		for (rapidxml::xml_node<> *gameObjectNode = scene->first_node("gameobject");
			gameObjectNode;
			gameObjectNode = gameObjectNode->next_sibling("gameobject"))
		{

			rapidxml::xml_attribute<> *nameAttribute = gameObjectNode->first_attribute("name");

			String name;

			if (nameAttribute)
			{
				name = nameAttribute->value();
			}

			GameObjectHandle hObj = m_gameobjects->Create(name);
			GameObject *gameObject = m_gameobjects->Get(hObj);

			rapidxml::xml_node<> *transform = gameObjectNode->first_node("transform");
			
			if (transform)
			{

				rapidxml::xml_node<> *positionNode = transform->first_node("position");
				rapidxml::xml_node<> *scaleNode = transform->first_node("scale");
				rapidxml::xml_node<> *orientationNode = transform->first_node("orientation");

				if (positionNode)
				{
					Vector3f position = ParseVector3<float>(positionNode->value());
					gameObject->GetTransformComponent()->SetPosition(position);
				}

				if (scaleNode)
				{
					Vector3f scale = ParseVector3<float>(scaleNode->value());
					gameObject->GetTransformComponent()->SetScale(scale);
				}
				
				if (orientationNode)
				{
					Quaternionf orientation = ParseQuaternion<float>(orientationNode->value());
					gameObject->GetTransformComponent()->SetOrientation(orientation);
				}

			}

			rapidxml::xml_node<> *camera = gameObjectNode->first_node("camera");

			if (camera)
			{

				gameObject->AddComponent(CameraComponent());

				rapidxml::xml_attribute<> *activeAttribute = camera->first_attribute("active");

				if (activeAttribute && String(activeAttribute->value()) == "true")
				{
					m_scene->SetCamera(gameObject->GetCameraComponent());
				}

				rapidxml::xml_node<> *positionNode = camera->first_node("position");

				if (positionNode)
				{
					Vector3f position = ParseVector3<float>(positionNode->value());
					gameObject->GetCameraComponent()->SetPosition(position);
				}

				rapidxml::xml_node<> *orientationNode = camera->first_node("orientation");

				if (orientationNode)
				{
					Quaternionf orientation = ParseQuaternion<float>(orientationNode->value());
					gameObject->GetCameraComponent()->SetOrientation(orientation);
				}

				rapidxml::xml_node<> *fovYNode = camera->first_node("fovy");

				if (fovYNode)
				{
					gameObject->GetCameraComponent()->SetFovY(ParseType<float>(fovYNode->value()));
				}

				rapidxml::xml_node<> *znear = camera->first_node("znear");

				if (znear)
				{
					gameObject->GetCameraComponent()->SetNearClipDistance(ParseType<float>(znear->value()));
				}

				rapidxml::xml_node<> *zfar = camera->first_node("zfar");

				if (zfar)
				{
					gameObject->GetCameraComponent()->SetFarClipDistance(ParseType<float>(zfar->value()));
				}

			}

			rapidxml::xml_node<> *render = gameObjectNode->first_node("render");

			if (render)
			{

				gameObject->AddComponent(RenderComponent());

				rapidxml::xml_node<> *aabbNode = render->first_node("aabb");

				if (aabbNode)
				{

					rapidxml::xml_node<> *aabbMin = aabbNode->first_node("min");
					rapidxml::xml_node<> *aabbMax = aabbNode->first_node("max");

					gameObject->GetRenderComponent()->SetBounds(AABBf::FromMinMax(
						ParseVector3<float>(aabbMin->value()),
						ParseVector3<float>(aabbMax->value())));

				}

				rapidxml::xml_node<> *modelNode = render->first_node("model");

				if (modelNode)
				{

					rapidxml::xml_attribute<> *modelName = modelNode->first_attribute("name");

					gameObject->GetRenderComponent()->SetModel(
						ResourceTypeManager::GetSingleton().CreateResource("Model", modelName->value()));

				}

			}

			m_scene->AddGameObject(gameObject->GetHandle());

			if (allocatedObjects)
			{
				allocatedObjects->push_back(gameObject);
			}

		}

	}

}