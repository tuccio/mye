namespace mye
{
	namespace core
	{
		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceTypeManager::CreateResource(const String &type,
			const String &name,
			ManualResourceLoader *manual,
			const Parameters &params)
		{

			ResourceManager *manager = GetResourceManager(type);

			if (manager)
			{
				return manager->CreateResource<ResourceType>(name, manual, params);
			}

			return std::shared_ptr<ResourceType>();

		}

		std::shared_ptr<Resource> ResourceTypeManager::CreateResource(const String &type,
			const String &name,
			ManualResourceLoader *manual,
			const Parameters &params)
		{
			return CreateResource<Resource>(type, name, manual, params);
		}

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceTypeManager::GetResource(const String &type,
			const String &name)
		{

			ResourceManager *manager = GetResourceManager(type);

			if (manager)
			{
				return manager->GetResource<ResourceType>(name);
			}

			return std::shared_ptr<ResourceType>();

		}

		std::shared_ptr<Resource> ResourceTypeManager::GetResource(const String &type,
			const String &name)
		{

			ResourceManager *manager = GetResourceManager(type);

			if (manager)
			{
				return manager->GetResource<Resource>(name);
			}

			return std::shared_ptr<Resource>();

		}

	}
}