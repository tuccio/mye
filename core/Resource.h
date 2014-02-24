#pragma once

#include "INamedObject.h"
#include "Lockable.h"

#include <unordered_map>

#define MYE_DEFAULT_GROUP ""
#define MYE_CONTAINS_PARAMETER(__PARAMSLIST, __PARAM) (__PARAMSLIST.find(__PARAM) != __PARAMSLIST.end())

namespace mye
{

	namespace core
	{

		class ManualResourceLoader;
		class ResourceManager;

		class Resource :
			public INamedObject,
			public Lockable
		{

		public:

			typedef std::unordered_map<String, String> ParametersList;
			
			enum LoadingState
			{
				RESOURCE_NOTLOADED,
				RESOURCE_LOADING,
				RESOURCE_PREPARING,
				RESOURCE_LOADED,
				RESOURCE_UNLOADING,
				RESOURCE_FREED
			};

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
			virtual bool PrepareImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void) = 0;

			ResourceManager *m_owner;
			ManualResourceLoader *m_manual;

			LoadingState m_loadingState;
			size_t m_size;

			ParametersList m_params;


		};

	}

}
