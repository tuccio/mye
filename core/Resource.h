#pragma once

#include "INamedObject.h"
#include "Lockable.h"

#include <unordered_map>
#include <boost/shared_ptr.hpp>

#define MYE_DEFAULT_GROUP ""
#define MYE_CONTAINS_PARAMETER(__PARAMSLIST, __PARAM) (__PARAMSLIST.find(__PARAM) != __PARAMSLIST.end())

namespace mye
{

	namespace core
	{

		enum class LoadingState
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

			typedef std::unordered_map<String, String> ParametersList;

			Resource(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~Resource(void);

			bool Load(bool background = false);
			void Unload(bool background = false);
			void Free(bool background = false);

			size_t GetSize(void) const;

			LoadingState GetState(void) const;

			void SetParametersList(const ParametersList &params);
			ParametersList GetParametersList(void) const;

		protected:

			friend class ManualResourceLoader;

			Resource(void);

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void) = 0;

			ResourceManager *m_owner;
			ManualResourceLoader *m_manual;

			LoadingState m_loadingState;
			size_t m_size;

			ParametersList m_params;


		};

		typedef boost::shared_ptr<Resource> ResourcePointer;

	}

}
