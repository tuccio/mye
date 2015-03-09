#pragma once

#include "EntityParser.h"
#include "EntityTypes.h"
#include "Sequence.h"
#include "String.h"

#include "GameObject.h"

#define __MYE_STATEMENT_VARIABLE(TYPE, VARIABLETYPE, CONSTRUCTIBLE, COMPONENT) \
	case VARIABLETYPE:\
	{\
		std::unique_ptr<COMPONENT> v(new COMPONENT(vd.name.c_str()));\
		if (vd.initializer)\
		{\
			auto & init = vd.initializer.get();\
			if (init.constructible == Constructible::IMPLICIT ||\
				init.constructible == CONSTRUCTIBLE)\
			{\
				*(v->GetPointer()) = std::move(ObjectInstantiator<TYPE>::Instantiate(init));\
			}\
			else\
			{\
				return nullptr;\
			}\
		}\
		else\
		{\
			*(v->GetPointer()) = std::move(ObjectInstantiator<TYPE>::Instantiate());\
		}\
		return v.release();\
	}\

#define __MYE_STATEMENT_COMPONENT(TYPE, COMPONENTTYPE) \
case COMPONENTTYPE:\
{\
	if (cd.initializer)\
	{\
		auto & init = cd.initializer.get();\
		if (init.constructible == Constructible::IMPLICIT)\
		{\
			return ObjectInstantiator<TYPE>::Instantiate(init).Clone();\
		}\
		else\
		{\
			return nullptr;\
		}\
	}\
	else\
	{\
		return ObjectInstantiator<TYPE>::Instantiate().Clone();\
	}\
}\
break;

namespace mye
{

	namespace core
	{

		namespace entity
		{

			/* Structures */

			struct StatementVisitor : boost::static_visitor<Component *>
			{

				Component * operator() (const VariableDefinition & vd) const
				{

					try
					{

						switch (vd.type)
						{

						__MYE_STATEMENT_VARIABLE(mye::math::Vector2, VariableTypes::VEC2, Constructible::VEC2, Vector2VariableComponent)
						__MYE_STATEMENT_VARIABLE(mye::math::Vector3, VariableTypes::VEC3, Constructible::VEC3, Vector3VariableComponent)
						__MYE_STATEMENT_VARIABLE(mye::math::Vector4, VariableTypes::VEC4, Constructible::VEC4, Vector4VariableComponent)

						__MYE_STATEMENT_VARIABLE(mye::math::Vector2i, VariableTypes::VEC2I, Constructible::VEC2I, Vector2iVariableComponent)
						__MYE_STATEMENT_VARIABLE(mye::math::Vector3i, VariableTypes::VEC3I, Constructible::VEC3I, Vector3iVariableComponent)
						__MYE_STATEMENT_VARIABLE(mye::math::Vector4i, VariableTypes::VEC4I, Constructible::VEC4I, Vector4iVariableComponent)

						__MYE_STATEMENT_VARIABLE(mye::math::Quaternion, VariableTypes::QUATERNION, Constructible::QUATERNION, QuaternionVariableComponent)

						__MYE_STATEMENT_VARIABLE(String, VariableTypes::STRING, Constructible::STRING, StringVariableComponent)

						__MYE_STATEMENT_VARIABLE(bool, VariableTypes::BOOL, Constructible::BOOL, BoolVariableComponent)
						__MYE_STATEMENT_VARIABLE(int, VariableTypes::INT, Constructible::INT, IntVariableComponent)
						__MYE_STATEMENT_VARIABLE(mye::math::Real, VariableTypes::REAL, Constructible::REAL, RealVariableComponent)


						__MYE_STATEMENT_VARIABLE(GameObjectHandle, VariableTypes::GAMEOBJECTHANDLE, Constructible::GAMEOBJECTHANDLE, GameObjectVariableComponent)

						default:
							break;

						}
					}
					catch (...) { }

					return nullptr;
				}

				Component * operator() (const ComponentDefinition & cd) const
				{
					
					switch (cd.component)
					{

						__MYE_STATEMENT_COMPONENT(RenderComponent,    ComponentTypes::RENDER)
						__MYE_STATEMENT_COMPONENT(TransformComponent, ComponentTypes::TRANSFORM)
						__MYE_STATEMENT_COMPONENT(CameraComponent,    ComponentTypes::CAMERA)
						__MYE_STATEMENT_COMPONENT(RigidBodyComponent, ComponentTypes::RIGIDBODY)
						__MYE_STATEMENT_COMPONENT(KeyboardComponent,  ComponentTypes::KEYBOARD)
						__MYE_STATEMENT_COMPONENT(MouseComponent,     ComponentTypes::MOUSE)
					

					}

					return nullptr;

				}

				GameObject * object;

			};

			struct EntityInstantiator
			{

				static bool Instantiate(GameObject * object, const EntityDefinition & e)
				{

					StatementVisitor visitor;

					for (auto & s : e.statements)
					{

						Component * c = s.apply_visitor(visitor);
						
						if (!c)
						{
							return false;
						}
						else
						{
							object->AddComponent(c);
						}

					}

					return true;

				}

			};

		}

	}

}