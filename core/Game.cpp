#include "Game.h"
#include "Logger.h"
#include "Stream.h"

#include "ResourceTypeManager.h"
#include "OctreeSceneModule.h"

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

#include <rapidxml_utils.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace mye::core;
using namespace mye::math;

#define __MYE_GAME_TRY_INIT_MODULE(x) if (!(x)->IsInitialized() && !(x)->Init()) return false;

Game::Game(InputModule       * input,
           GameObjectsModule * gameobjects,
           SceneModule       * scene,
           PhysicsModule     * physics,
           GraphicsModule    * graphics,
           AudioModule       * audio,
           ScriptModule      * script) :
	m_input(input),
	m_gameobjects(gameobjects),
	m_scene(scene),
	m_physics(physics),
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

	m_eventManager = new EventManager;

	__MYE_GAME_TRY_INIT_MODULE(m_gameobjects)
	__MYE_GAME_TRY_INIT_MODULE(m_scene)
	__MYE_GAME_TRY_INIT_MODULE(m_graphics)
	__MYE_GAME_TRY_INIT_MODULE(m_physics)
	__MYE_GAME_TRY_INIT_MODULE(m_audio)
	__MYE_GAME_TRY_INIT_MODULE(m_input)
	__MYE_GAME_TRY_INIT_MODULE(m_script)

	return true;

}

void Game::Run(void)
{
	m_timer.Start();
}

void Game::Quit(void)
{
	m_timer.Stop();
}

InputModule * Game::GetInputModule(void)
{
	return m_input;
}

GameObjectsModule * Game::GetGameObjectsModule(void)
{
	return m_gameobjects;
}

SceneModule * Game::GetSceneModule(void)
{
	return m_scene;
}

PhysicsModule * Game::GetPhysicsModule(void)
{
	return m_physics;
}

GraphicsModule * Game::GetGraphicsModule(void)
{
	return m_graphics;
}

AudioModule * Game::GetAudioModule(void)
{
	return m_audio;
}

ScriptModule * Game::GetScriptModule(void)
{
	return m_script;
}

void Game::RuntimeError(const String &error)
{
	mye::core::Logger::LogErrorOptional(error);
}

void Game::ExportScene(const String & path)
{

	FileOutputStream f(path);

	if (f)
	{

		rapidxml::xml_document<> document;

		rapidxml::xml_node<> * scene = document.allocate_node(rapidxml::node_element, "scene");

		document.append_node(scene);

		CameraComponent * activeCamera = static_cast<CameraComponent*>(m_scene->GetCamera());

		GameObjectsList list = m_scene->GetObjectsList();

		for (GameObject * o : list)
		{

			rapidxml::xml_node<> * gameObjectNode = document.allocate_node(
				rapidxml::node_element,
				"gameobject");

			scene->append_node(gameObjectNode);

			gameObjectNode->append_attribute(document.allocate_attribute("name",   o->GetName().CString()));
			gameObjectNode->append_attribute(document.allocate_attribute("entity", o->GetEntityType().CString()));

			for (auto it : *o)
			{

				switch (it.second->GetComponentType())
				{

				case ComponentTypes::TRANSFORM:

					{

						TransformComponent * tc = static_cast<TransformComponent*>(it.second);

						rapidxml::xml_node<> * transformNode = document.allocate_node(rapidxml::node_element, "transform");

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

						rapidxml::xml_node<> * positionNode = document.allocate_node(rapidxml::node_element,
																					 "position",
																					 document.allocate_string(position.CString()));

						rapidxml::xml_node<> * scaleNode = document.allocate_node(rapidxml::node_element,
																				  "scale",
																				  document.allocate_string(scale.CString()));

						rapidxml::xml_node<> * orientationNode = document.allocate_node(rapidxml::node_element,
																						"orientation",
																						document.allocate_string(orientation.CString()));

						transformNode->append_node(positionNode);
						transformNode->append_node(scaleNode);
						transformNode->append_node(orientationNode);

					}

					break;

				case ComponentTypes::CAMERA:

					{

						CameraComponent * cc = static_cast<CameraComponent*>(it.second);

						rapidxml::xml_node<> * cameraNode = document.allocate_node(
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

						rapidxml::xml_node<> * positionNode = document.allocate_node(rapidxml::node_element,
																					 "position",
																					 document.allocate_string(position.CString()));

						rapidxml::xml_node<> * orientationNode = document.allocate_node(rapidxml::node_element,
																						"orientation",
																						document.allocate_string(orientation.CString()));

						rapidxml::xml_node<> * fovYNode = document.allocate_node(rapidxml::node_element,
																				 "fovy",
																				 document.allocate_string(ToString(cc->GetFovY()).CString()));

						rapidxml::xml_node<> * znearNode = document.allocate_node(rapidxml::node_element,
																				  "znear",
																				  document.allocate_string(ToString(cc->GetNearClipDistance()).CString()));

						rapidxml::xml_node<> * zfarNode = document.allocate_node(rapidxml::node_element,
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

						RenderComponent * rc = static_cast<RenderComponent*>(it.second);

						rapidxml::xml_node<> *renderNode = document.allocate_node(rapidxml::node_element, "render");

						gameObjectNode->append_node(renderNode);

						String AABBmin = (ToString(rc->GetBounds().GetMinimum().x()) +
							" " +
							ToString(rc->GetBounds().GetMinimum().y()) +
							" " +
							ToString(rc->GetBounds().GetMinimum().z()));

						String AABBmax = (ToString(rc->GetBounds().GetMaximum().x()) +
							" " +
							ToString(rc->GetBounds().GetMaximum().y()) +
							" " +
							ToString(rc->GetBounds().GetMaximum().z()));

						rapidxml::xml_node<> *AABB = document.allocate_node(rapidxml::node_element,	"aabb");

						rapidxml::xml_node<> *mesh = document.allocate_node(rapidxml::node_element, "mesh");

						mesh->append_attribute(document.allocate_attribute("name", 
							document.allocate_string(rc->GetMesh().get()->GetName().CString())));

						AABB->append_node(document.allocate_node(rapidxml::node_element,
							"min",
							document.allocate_string(AABBmin.CString())));

						AABB->append_node(document.allocate_node(rapidxml::node_element,
							"max",
							document.allocate_string(AABBmax.CString())));

						renderNode->append_node(AABB);
						renderNode->append_node(mesh);

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

void Game::ImportScene(const String & file, std::list<GameObject*> * allocatedObjects)
{

	rapidxml::file<> xmlFile(file.CString());

	rapidxml::xml_document<> document;
	document.parse<0>(xmlFile.data());

	rapidxml::xml_node<> * scene = document.first_node("scene");

	if (scene)
	{

		rapidxml::xml_attribute<> * module = scene->first_attribute("module");

		if (module && !strcmp(module->value(), "octree"))
		{

			rapidxml::xml_attribute<> * centerNode = scene->first_attribute("center");
			rapidxml::xml_attribute<> * sizeNode = scene->first_attribute("size");
			rapidxml::xml_attribute<> * maxdepthNode = scene->first_attribute("maxdepth");

			Vector3 center(0.0f);
			float size = 1024.0f;
			unsigned int maxdepth = 32;

			if (centerNode)
			{
				center = ParseVector3<float>(centerNode->value());
			}

			if (sizeNode)
			{
				size = ParseType<float>(sizeNode->value());
			}

			if (maxdepthNode)
			{
				maxdepth = ParseType<unsigned int>(maxdepthNode->value());
			}

			OctreeSceneModule * octreeScene = static_cast<OctreeSceneModule*>(m_scene);

			octreeScene->Reset(center, size, maxdepth);

		}

		for (rapidxml::xml_node<> * gameObjectNode = scene->first_node("gameobject");
			gameObjectNode;
			gameObjectNode = gameObjectNode->next_sibling("gameobject"))
		{

			rapidxml::xml_attribute<> * nameAttribute = gameObjectNode->first_attribute("name");

			String name;

			if (nameAttribute)
			{
				name = nameAttribute->value();
			}

			rapidxml::xml_attribute<> * entityAttribute = gameObjectNode->first_attribute("entity");

			String entity;

			if (entityAttribute)
			{
				entity = entityAttribute->value();
			}

			GameObjectHandle hObj = (entity.Length() ? m_gameobjects->CreateEntity(entity, name) : m_gameobjects->Create(name));
			GameObject * gameObject = m_gameobjects->Get(hObj);

			if (gameObject)
			{
				
				/*
				 * Transform component
				 */

				rapidxml::xml_node<> * transform = gameObjectNode->first_node("transform");
				
				if (transform)
				{

					rapidxml::xml_node<> * positionNode    = transform->first_node("position");
					rapidxml::xml_node<> * scaleNode       = transform->first_node("scale");
					rapidxml::xml_node<> * orientationNode = transform->first_node("orientation");

					if (positionNode)
					{
						Vector3 position = ParseVector3<float>(positionNode->value());
						gameObject->GetTransformComponent()->SetPosition(position);
					}

					if (scaleNode)
					{
						Vector3 scale = ParseVector3<float>(scaleNode->value());
						gameObject->GetTransformComponent()->SetScale(scale);
					}
					
					if (orientationNode)
					{
						Quaternion orientation = ParseQuaternion<float>(orientationNode->value());
						gameObject->GetTransformComponent()->SetOrientation(orientation);
					}

				}

				/*
				 * Camera component
				 */

				rapidxml::xml_node<> * camera = gameObjectNode->first_node("camera");

				if (camera)
				{

					gameObject->AddComponent(CameraComponent());

					rapidxml::xml_attribute<> * activeAttribute = camera->first_attribute("active");

					if (activeAttribute && String(activeAttribute->value()) == "true")
					{
						m_scene->SetCamera(gameObject->GetCameraComponent());
					}

					rapidxml::xml_node<> * positionNode = camera->first_node("position");

					if (positionNode)
					{
						Vector3 position = ParseVector3<float>(positionNode->value());
						gameObject->GetCameraComponent()->SetPosition(position);
					}

					rapidxml::xml_node<> * orientationNode = camera->first_node("orientation");

					if (orientationNode)
					{
						Quaternion orientation = ParseQuaternion<float>(orientationNode->value());
						gameObject->GetCameraComponent()->SetOrientation(orientation);
					}

					rapidxml::xml_node<> * fovYNode = camera->first_node("fovy");

					if (fovYNode)
					{
						gameObject->GetCameraComponent()->SetFovY(ParseType<float>(fovYNode->value()));
					}

					rapidxml::xml_node<> * znear = camera->first_node("znear");

					if (znear)
					{
						gameObject->GetCameraComponent()->SetNearClipDistance(ParseType<float>(znear->value()));
					}

					rapidxml::xml_node<> * zfar = camera->first_node("zfar");

					if (zfar)
					{
						gameObject->GetCameraComponent()->SetFarClipDistance(ParseType<float>(zfar->value()));
					}

				}

				/*
				 * Render component
				 */

				rapidxml::xml_node<> * render = gameObjectNode->first_node("render");

				if (render)
				{

					gameObject->AddComponent(RenderComponent());

					rapidxml::xml_node<> * meshNode = render->first_node("mesh");

					if (meshNode)
					{

						rapidxml::xml_attribute<> * modelName = meshNode->first_attribute("name");

						MeshPointer mesh = ResourceTypeManager::GetSingleton().
							CreateResource<Mesh>("Mesh", modelName->value());

						gameObject->GetRenderComponent()->SetMesh(mesh);

					}

					rapidxml::xml_node<> * AABBnode = render->first_node("aabb");

					if (AABBnode)
					{

						rapidxml::xml_node<> * AABBmin = AABBnode->first_node("min");
						rapidxml::xml_node<> * AABBmax = AABBnode->first_node("max");

						gameObject->GetRenderComponent()->SetBounds(AABB::FromMinMax(
							ParseVector3<mye::math::Real>(AABBmin->value()),
							ParseVector3<mye::math::Real>(AABBmax->value())));

					}
					else if (meshNode)
					{
						auto minMax = gameObject->GetRenderComponent()->GetMesh()->GetMinMaxVertices();
						gameObject->GetRenderComponent()->SetBounds(AABB::FromMinMax(minMax.first, minMax.second));
					}

					rapidxml::xml_node<> * materialNode = render->first_node("material");

					if (materialNode)
					{

						rapidxml::xml_attribute<> * diffuse_color = materialNode->first_attribute("diffuse_color");
						rapidxml::xml_attribute<> * roughness     = materialNode->first_attribute("roughness");
						rapidxml::xml_attribute<> * specular      = materialNode->first_attribute("specular");
						rapidxml::xml_attribute<> * metallic      = materialNode->first_attribute("metallic");

						MaterialPointer material(new Material(nullptr, "", nullptr));

						material->SetDiffuseColor(ParseVector4<mye::math::Real>(diffuse_color->value()));
						material->SetRoughness(ParseType<mye::math::Real>(roughness->value()));
						material->SetSpecular(ParseType<mye::math::Real>(specular->value()));
						material->SetMetallic(ParseType<mye::math::Real>(metallic->value()));

						gameObject->GetRenderComponent()->SetMaterial(material);

					}

					

				}

				/*
				 * Light component
				 */

				rapidxml::xml_node<> * light = gameObjectNode->first_node("light");

				if (light)
				{

					LightComponent * lightComponent = static_cast<LightComponent*>(gameObject->AddComponent(LightComponent()));

					rapidxml::xml_attribute<> * type = light->first_attribute("type");

					if (!strcmp(type->value(), "pointlight"))
					{
						lightComponent->SetType(LightType::POINTLIGHT);
					}
					else if (!strcmp(type->value(), "spotlight"))
					{
						lightComponent->SetType(LightType::SPOTLIGHT);
					}
					else if (!strcmp(type->value(), "directional"))
					{
						lightComponent->SetType(LightType::DIRECTIONAL);
					}

					rapidxml::xml_node<> * position  = light->first_node("position");
					rapidxml::xml_node<> * color     = light->first_node("color");
					rapidxml::xml_node<> * intensity = light->first_node("intensity");
					rapidxml::xml_node<> * range     = light->first_node("range");
					rapidxml::xml_node<> * direction = light->first_node("direction");
					rapidxml::xml_node<> * spotAngle = light->first_node("spotAngle");

					if (position)
					{
						lightComponent->SetPosition(ParseVector3<mye::math::Real>(position->value()));
					}

					if (color)
					{
						lightComponent->SetColor(ParseVector3<mye::math::Real>(color->value()));
					}

					if (intensity)
					{
						lightComponent->SetIntensity(ParseType<mye::math::Real>(intensity->value()));
					}

					if (range)
					{
						lightComponent->SetRange(ParseType<mye::math::Real>(range->value()));
					}

					if (direction)
					{
						lightComponent->SetDirection(ParseVector3<mye::math::Real>(direction->value()));
					}

					if (spotAngle)
					{
						lightComponent->SetSpotAngle(ParseType<mye::math::Real>(spotAngle->value()));
					}

				}

				if (gameObject->GetRenderComponent())
				{
					m_scene->AddGameObject(gameObject->GetHandle());
				}

				if (allocatedObjects)
				{
					allocatedObjects->push_back(gameObject);
				}

			}

		}

	}

}