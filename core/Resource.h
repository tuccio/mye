#pragma once

#include "INamedObject.h"
#include "Lockable.h"
#include "Parameters.h"

#include <unordered_map>
#include <boost/shared_ptr.hpp>

#define MYE_DEFAULT_GROUP ""
#define MYE_CONTAINS_PARAMETER(__PARAMSLIST, __PARAM) (__PARAMSLIST.find(__PARAM) != __PARAMSLIST.end())

namespace mye
{

	namespace core
	{

		enum class ResourceLoadState
		{
			NOT_LOADED,
			LOADING,
			LOADED,
			UNLOADING,
			FREED
		};

		class ManualResourceLoader;
		class ResourceManager;

		class Resource :
			public INamedObject,
			public Lockable
		{

		public:

			template <typename T, typename R>
			inline static boost::shared_ptr<T> StaticCast(boost::shared_ptr<R> r)
			{
				return boost::static_pointer_cast<T, R>(r);
			}

			template <typename T, typename R>
			inline static boost::shared_ptr<T> DynamicCast(boost::shared_ptr<R> r)
			{
				return boost::dynamic_pointer_cast<T, R>(r);
			}

			template <typename T, typename R>
			inline static boost::shared_ptr<T> ReinterpretCast(boost::shared_ptr<R> r)
			{
				return boost::reinterpret_pointer_cast<T, R>(r);
			}

			Resource(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~Resource(void);

			bool Load(bool background = false);
			void Unload(bool background = false);
			void Free(bool background = false);

			void CalculateSize(void);

			size_t GetSize(void) const;

			ResourceLoadState GetState(void) const;

			void SetParametersList(const Parameters &params);
			Parameters GetParametersList(void) const;

		protected:

			friend class ManualResourceLoader;

			Resource(void);

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void) = 0;

			ResourceManager *m_owner;
			ManualResourceLoader *m_manual;

			ResourceLoadState m_loadingState;
			size_t m_size;

			Parameters m_params;


		};

		typedef boost::shared_ptr<Resource> ResourcePointer;

	}

}
