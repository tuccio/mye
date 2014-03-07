namespace mye
{
	namespace core
	{
		template <typename ResourceType>
		boost::shared_ptr<ResourceType> ResourceTypeManager::CreateResource(const String &type,
			const String &name,
			ManualResourceLoader *manual,
			Resource::ParametersList *params)
		{

			ResourceManager *manager = GetResourceManager(type);

			if (manager)
			{
				return manager->CreateResource<ResourceType>(name, manual, params);
			}

			return boost::shared_ptr<ResourceType>();

		}

		boost::shared_ptr<Resource> ResourceTypeManager::CreateResource(const String &type,
			const String &name,
			ManualResourceLoader *manual,
			Resource::ParametersList *params)
		{
			return CreateResource(type, name, manual, params);
		}

		template <typename ResourceType>
		boost::shared_ptr<ResourceType> ResourceTypeManager::GetResource(const String &type,
			const String &name)
		{
			ResourceManager *manager = GetResourceManager(type);

			if (manager)
			{
				return manager->GetResource<ResourceType>(name);
			}

			return boost::shared_ptr<ResourceType>();
		}

		boost::shared_ptr<Resource> ResourceTypeManager::GetResource(const String &type,
			const String &name)
		{
			return GetResource(type, name);
		}

	}
}