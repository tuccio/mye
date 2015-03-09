#pragma once

#include "Components.h"
#include "EntityParser.h"
#include "Sequence.h"

#include "ResourceTypeManager.h"

#include <type_traits>

#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		namespace entity
		{

			struct NoMatchingConstructorException :
				std::exception
			{

				NoMatchingConstructorException(const char * what) :
					std::exception(what) { }

			};


			template <typename T>
			struct TypeMatcher;

			/* Bool */

			template <>
			struct TypeMatcher<bool>
			{

				static bool Match(const InitValue & init)
				{

					bool match = false;

					if (init.constructible == Constructible::BOOL)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								match = Match(init.initializers[0]);
							}

						}
						else if (init.value.type() == typeid(bool))
						{
							match = true;
						}

					}

					return match;

				}

				static bool Evaluate(const InitValue & init)
				{

					bool result;

					auto initializers = init.initializers.size();

					if (initializers)
					{

						if (initializers == 1)
						{
							result = Evaluate(init.initializers[0]);
						}

					}
					else
					{
						result = boost::get<bool>(init.value);
					}

					return result;

				}

			};

			/* Integer */

			template <>
			struct TypeMatcher<int>
			{

				static bool Match(const InitValue & init)
				{

					bool match = false;

					if (init.constructible == Constructible::INT)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								match = Match(init.initializers[0]);
							}

						}
						else if (init.value.type() == typeid(int))
						{
							match = true;
						}

					}

					return match;

				}

				static int Evaluate(const InitValue & init)
				{

					int result;

					auto initializers = init.initializers.size();

					if (initializers)
					{

						if (initializers == 1)
						{
							result = Evaluate(init.initializers[0]);
						}

					}
					else
					{
						result = boost::get<int>(init.value);
					}

					return result;

				}

			};

			/* Real */

			template <>
			struct TypeMatcher<double>
			{

				static bool Match(const InitValue & init)
				{

					bool match = false;

					if (init.constructible == Constructible::REAL)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								match = Match(init.initializers[0]);
							}

						}
						else if (init.value.type() == typeid(double))
						{
							match = true;
						}

					}
					else
					{
						match = TypeMatcher<int>::Match(init);
					}

					return match;

				}

				static double Evaluate(const InitValue & init)
				{

					double result;

					auto initializers = init.initializers.size();

					if (init.constructible == Constructible::REAL)
					{

						if (initializers)
						{

							if (initializers == 1)
							{
								result = Evaluate(init.initializers[0]);
							}

						}
						else
						{
							result = boost::get<double>(init.value);
						}

					}
					else
					{
						result = TypeMatcher<int>::Evaluate(init);
					}

					return result;

				}

			};

			template <>
			struct TypeMatcher<float> :
				public TypeMatcher<double> { };

			/* String */

			template <>
			struct TypeMatcher<std::string>
			{

				static bool Match(const InitValue & init)
				{

					bool match = false;

					if (init.constructible == Constructible::STRING)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								match = Match(init.initializers[0]);
							}

						}
						else if (init.value.type() == typeid(std::string))
						{
							match = true;
						}

					}

					return match;

				}

				static std::string Evaluate(const InitValue & init)
				{

					std::string result;

					auto initializers = init.initializers.size();

					if (initializers)
					{

						if (initializers == 1)
						{
							result = Evaluate(init.initializers[0]);
						}

					}
					else
					{
						result = boost::get<std::string>(init.value);
					}

					return result;

				}

			};

			template <>
			struct TypeMatcher<String>
			{
				
				static bool Match(const InitValue & init)
				{
					return TypeMatcher<std::string>::Match(init);
				}

				static String Evaluate(const InitValue &init)
				{
					return TypeMatcher<std::string>::Evaluate(init).c_str();
				}

			};

			/* Parameters */

			template <>
			struct TypeMatcher<Parameters>
			{

				static bool Match(const InitValue & init)
				{

					bool match = false;

					if (init.constructible == Constructible::PARAMETERS)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								match = Match(init.initializers[0]);
							}

						}
						else if (init.value.type() == typeid(Parameters))
						{
							match = true;
						}

					}
					else if (init.constructible == Constructible::NIL)
					{
						match = true;
					}

					return match;

				}

				static Parameters Evaluate(const InitValue & init)
				{

					Parameters result;

					if (init.constructible == Constructible::PARAMETERS)
					{

						auto initializers = init.initializers.size();

						if (initializers)
						{

							if (initializers == 1)
							{
								result = Evaluate(init.initializers[0]);
							}

						}
						else
						{
							result = boost::get<Parameters>(init.value);
						}

					}

					return result;

				}

			};

			/* Null */

			template <>
			struct TypeMatcher<std::nullptr_t>
			{

				static bool Match(const InitValue & init)
				{
					return init.constructible == Constructible::NIL;
				}

				static std::nullptr_t Evaluate(const InitValue & init)
				{
					return nullptr;
				}

			};

			/* ObjectInstantiator declaration */

			template <typename C>
			struct ObjectInstantiator;

			/* Constructors */

			template <typename ... Args>
			struct ConstructorMatcher;

			template <typename Arg, typename ... Args>
			struct ConstructorMatcher<Arg, Args ...>
			{

				template <typename Iterator>
				static bool Match(Iterator it, Iterator end)
				{

					if (it != end)
					{
						return ObjectInstantiator<Arg>::Match(*it) && ConstructorMatcher<Args ...>::Match(it + 1, end);
					}
					else
					{
						return false;
					}

				}


			};

			template <>
			struct ConstructorMatcher<>
			{

				template <typename Iterator>
				static bool Match(Iterator it, Iterator end)
				{
					return it == end;
				}

			};

			template <typename C, typename ... Args>
			inline C Construct(Args && ... args)
			{
				return C(std::forward<Args>(args) ...);
			}

			template <typename C, typename ... Args>
			struct ConstructorCaller
			{

				template <typename Iterator, int ... N>
				static C Call(Iterator it, Iterator end, IntegerSequence<N ...>)
				{
					std::tuple<Args ...> args = MakeTuple(it, IntegerSequence<N ...>());
					return Construct<C>(std::get<N>(std::forward<std::tuple<Args ...>>(args)) ...);
				}

				template <typename Iterator, int ... N>
				static std::tuple<Args ...> MakeTuple(Iterator it, IntegerSequence<N ...>)
				{
					return std::forward_as_tuple(ObjectInstantiator<Args>::Instantiate(* (it + N)) ...);
				}

			};

			/* Object Instantiator */

			template <typename T>
			struct ObjectInstantiator
			{

				static bool Match(const InitValue & init)
				{
					return TypeMatcher<T>::Match(init);
				}

				static T Instantiate(void)
				{
					return T();
				}

				static T Instantiate(const InitValue & init)
				{

					if (TypeMatcher<T>::Match(init))
					{
						return TypeMatcher<T>::Evaluate(init);
					}
					else
					{
						throw NoMatchingConstructorException(std::type_index(typeid(T)).name());
					}

					return T();

				}

			};

			template <typename C, typename ... Args>
			struct Constructor
			{

				template <typename Iterator>
				inline static bool Match(Iterator it, Iterator end)
				{
					return std::distance(it, end) == sizeof ... (Args) && ConstructorMatcher<Args ...>::Match(it, end);
				}

				template <typename Iterator>
				inline static C Call(Iterator it, Iterator end)
				{
					return ConstructorCaller<C, Args ...>::Call(it, end, IntegerSequenceGenerator<sizeof ... (Args)>::type());
				}

				inline static C Call(void)
				{
					return Construct<C>();
				}

			};

			template <typename R>
			struct ResourceTypeMap;

			template <typename C>
			struct ResourceConstructor
			{

				template <typename Iterator>
				inline static bool Match(Iterator it, Iterator end)
				{
					return ConstructorMatcher<String, Parameters>::Match(it, end);
				}

				template <typename Iterator>
				inline static std::shared_ptr<C> Call(Iterator it, Iterator end)
				{

					return ResourceTypeManager::GetSingleton().CreateResource<C>(ResourceTypeMap<C>::type,
																				 TypeMatcher<String>::Evaluate(*it),
																				 nullptr,
																				 TypeMatcher<Parameters>::Evaluate(*(it + 1)));

				}

			};

			template <typename C, typename ... Constructors>
			struct ObjectFactory;

			template <typename C, typename Ctor, typename Ctor2, typename ... Constructors>
			struct ObjectFactory<C, Ctor, Ctor2, Constructors ...>
			{

				template <typename Iterator>
				static bool Match(Iterator it, Iterator end)
				{
					if (Ctor::Match(it, end))
					{
						return true;
					}
					else
					{
						return ObjectFactory<C, Ctor2, Constructors ...>::Match(it, end);
					}
				}

				template <typename Iterator>
				static C Construct(Iterator it, Iterator end)
				{
					if (Ctor::Match(it, end))
					{
						return Ctor::Call(it, end);
					}
					else
					{
						return ObjectFactory<C, Ctor2, Constructors ...>::Construct(it, end);
					}
				}

			};

			template <typename C, typename Ctor>
			struct ObjectFactory<C, Ctor>
			{

				template <typename Iterator>
				static bool Match(Iterator it, Iterator end)
				{
					if (Ctor::Match(it, end))
					{
						return true;
					}
					else
					{
						return false;
					}
				}

				template <typename Iterator>
				static C Construct(Iterator it, Iterator end)
				{
					if (Ctor::Match(it, end))
					{
						return Ctor::Call(it, end);
					}
					else
					{
						throw;
					}
				}

			};

			/* Vector2 */

			template <>
			struct ObjectInstantiator<mye::math::Vector2>
			{

				typedef ObjectFactory
					<
					mye::math::Vector2,
					Constructor<mye::math::Vector2>,
					Constructor<mye::math::Vector2, mye::math::Real>,
					Constructor<mye::math::Vector2, mye::math::Real, mye::math::Real>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector2 Instantiate(void)
				{
					return Constructor<mye::math::Vector2>::Call();
				}

				static mye::math::Vector2 Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			template <>
			struct ObjectInstantiator<mye::math::Vector2i>
			{

				typedef ObjectFactory
					<
					mye::math::Vector2i,
					Constructor<mye::math::Vector2i>,
					Constructor<mye::math::Vector2i, int>,
					Constructor<mye::math::Vector2i, int, int>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector2i Instantiate(void)
				{
					return Constructor<mye::math::Vector2i>::Call();
				}

				static mye::math::Vector2i Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Vector3 */

			template <>
			struct ObjectInstantiator<mye::math::Vector3>
			{

				typedef ObjectFactory
					<
					mye::math::Vector3,
					Constructor<mye::math::Vector3>,
					Constructor<mye::math::Vector3, mye::math::Real>,
					Constructor<mye::math::Vector3, mye::math::Real, mye::math::Real, mye::math::Real>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector3 Instantiate(void)
				{
					return Constructor<mye::math::Vector3>::Call();
				}

				static mye::math::Vector3 Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			template <>
			struct ObjectInstantiator<mye::math::Vector3i>
			{

				typedef ObjectFactory
					<
					mye::math::Vector3i,
					Constructor<mye::math::Vector3i>,
					Constructor<mye::math::Vector3i, int>,
					Constructor<mye::math::Vector3i, int, int, int>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector3i Instantiate(void)
				{
					return Constructor<mye::math::Vector3i>::Call();
				}

				static mye::math::Vector3i Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Vector4 */

			template <>
			struct ObjectInstantiator<mye::math::Vector4>
			{

				typedef ObjectFactory
					<
					mye::math::Vector4,
					Constructor<mye::math::Vector4>,
					Constructor<mye::math::Vector4, mye::math::Real>,
					Constructor<mye::math::Vector4, mye::math::Real, mye::math::Real, mye::math::Real, mye::math::Real>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector4 Instantiate(void)
				{
					return Constructor<mye::math::Vector4>::Call();
				}

				static mye::math::Vector4 Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			template <>
			struct ObjectInstantiator<mye::math::Vector4i>
			{

				typedef ObjectFactory
					<
					mye::math::Vector4i,
					Constructor<mye::math::Vector4i>,
					Constructor<mye::math::Vector4i, int>,
					Constructor<mye::math::Vector4i, int, int, int, int>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Vector4i Instantiate(void)
				{
					return Constructor<mye::math::Vector4i>::Call();
				}

				static mye::math::Vector4i Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Quaternion */

			template <>
			struct ObjectInstantiator<mye::math::Quaternion>
			{

				typedef ObjectFactory
					<
					mye::math::Quaternion,
					Constructor<mye::math::Quaternion>,
					Constructor<mye::math::Quaternion, mye::math::Real, mye::math::Real, mye::math::Real, mye::math::Real>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Quaternion Instantiate(void)
				{
					return Constructor<mye::math::Quaternion>::Call();
				}

				static mye::math::Quaternion Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Matrix4 */

			template <>
			struct ObjectInstantiator<mye::math::Matrix4>
			{

				typedef ObjectFactory
					<
					mye::math::Matrix4,
					Constructor<mye::math::Matrix4>,
					Constructor<mye::math::Matrix4, mye::math::Real>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static mye::math::Matrix4 Instantiate(void)
				{
					return Constructor<mye::math::Matrix4>::Call();
				}

				static mye::math::Matrix4 Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* String */

			/*template <>
			struct ObjectInstantiator<String>
			{

				typedef ObjectFactory
					<
					String,
					Constructor<String>,
					Constructor<String, String>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static String Instantiate(void)
				{
					return Constructor<String>::Call();
				}

				static String Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};*/

			/* GameObject */

			template <>
			struct ObjectInstantiator<GameObjectHandle>
			{

				typedef ObjectFactory
					<
					GameObjectHandle,
					Constructor<GameObjectHandle>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static GameObjectHandle Instantiate(void)
				{
					return Constructor<GameObjectHandle>::Call();
				}

				static GameObjectHandle Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Parameters */

			template <>
			struct ObjectInstantiator<Parameters>
			{

				typedef ObjectFactory
					<
					Parameters,
					Constructor<Parameters>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static Parameters Instantiate(void)
				{
					return Constructor<Parameters>::Call();
				}

				static Parameters Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* Mesh */

			template <>
			struct ObjectInstantiator<MeshPointer>
			{

				static bool Match(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return true;
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<Mesh>::Match(it, end);

				}

				static MeshPointer Instantiate(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return MeshPointer();
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<Mesh>::Call(it, end);

				}

			};

			template <>
			struct ResourceTypeMap<Mesh>
			{
				static const char * type;
			};

			/* Material */

			template <>
			struct ObjectInstantiator<MaterialPointer>
			{

				static bool Match(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return true;
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<Material>::Match(it, end);

				}

				static MaterialPointer Instantiate(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return MaterialPointer();
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<Material>::Call(it, end);

				}

			};			

			template <>
			struct ResourceTypeMap<Material>
			{
				static const char * type;
			};

			/* BulletCollisionShape */

			template <>
			struct ObjectInstantiator<BulletCollisionShapePointer>
			{

				static bool Match(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return true;
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<BulletCollisionShape>::Match(it, end);

				}

				static BulletCollisionShapePointer Instantiate(const InitValue & init)
				{

					if (init.constructible == Constructible::NIL)
					{
						return BulletCollisionShapePointer();
					}

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return ResourceConstructor<BulletCollisionShape>::Call(it, end);

				}

			};

			template <>
			struct ResourceTypeMap<BulletCollisionShape>
			{
				static const char * type;
			};

			/* RenderComponent */

			template <>
			struct ObjectInstantiator<RenderComponent>
			{

				typedef ObjectFactory
					<
					RenderComponent,
					Constructor<RenderComponent>,
					Constructor<RenderComponent, MeshPointer, MaterialPointer, mye::math::Matrix4>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static RenderComponent Instantiate(void)
				{
					return Constructor<RenderComponent>::Call();
				}

				static RenderComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* TransformComponent */

			template <>
			struct ObjectInstantiator<TransformComponent>
			{

				typedef ObjectFactory
					<
					TransformComponent,
					Constructor<TransformComponent>,
					Constructor<TransformComponent, mye::math::Vector3, mye::math::Quaternion, mye::math::Vector3>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static TransformComponent Instantiate(void)
				{
					return Constructor<TransformComponent>::Call();
				}

				static TransformComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* CameraComponent */

			template <>
			struct ObjectInstantiator<CameraComponent>
			{

				typedef ObjectFactory
					<
					CameraComponent,
					Constructor<CameraComponent>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static CameraComponent Instantiate(void)
				{
					return Constructor<CameraComponent>::Call();
				}

				static CameraComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* RigidBodyComponent */

			template <>
			struct ObjectInstantiator<RigidBodyComponent>
			{

				typedef ObjectFactory
					<
					RigidBodyComponent,
					Constructor<RigidBodyComponent>,
					Constructor<RigidBodyComponent, BulletCollisionShapePointer, mye::math::Real>,
					Constructor<RigidBodyComponent, BulletCollisionShapePointer, mye::math::Real, mye::math::Vector3, mye::math::Quaternion>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static RigidBodyComponent Instantiate(void)
				{
					return Constructor<RigidBodyComponent>::Call();
				}

				static RigidBodyComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* KeyboardComponent */

			template <>
			struct ObjectInstantiator<KeyboardComponent>
			{

				typedef ObjectFactory
					<
					KeyboardComponent,
					Constructor<KeyboardComponent>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static KeyboardComponent Instantiate(void)
				{
					return Constructor<KeyboardComponent>::Call();
				}

				static KeyboardComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

			/* MouseComponent */
			
			template <>
			struct ObjectInstantiator<MouseComponent>
			{

				typedef ObjectFactory
					<
					MouseComponent,
					Constructor<MouseComponent>
					>
					factory;

				static bool Match(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Match(it, end);

				}

				static MouseComponent Instantiate(void)
				{
					return Constructor<MouseComponent>::Call();
				}

				static MouseComponent Instantiate(const InitValue & init)
				{

					auto it  = init.initializers.cbegin();
					auto end = init.initializers.cend();

					return factory::Construct(it, end);

				}

			};

		}

	}

}