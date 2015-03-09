namespace mye
{

	namespace core
	{

		std::shared_ptr<Resource> ResourceManager::CreateResource(const String &name,
			ManualResourceLoader *manual,
			const Parameters &params)
		{
			return CreateResource<Resource>(name, manual, params);
		}

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::CreateResource(const String &name,
			ManualResourceLoader *manual,
			const Parameters &params)
		{

			Lock();

			auto it = m_resources.find(name);

			if (it != m_resources.end())
			{
				return Resource::StaticCast<ResourceType>(it->second);
			}

			Resource * r = CreateImpl(name, manual, params);
			r->SetParametersList(params);

			auto it2 = m_resources.insert(std::pair<String, ResourcePointer>(name, ResourcePointer(r)));
			
			Unlock();

			return Resource::StaticCast<ResourceType>(it2.first->second);

		}

		std::shared_ptr<Resource> ResourceManager::GetResource(const String &name)
		{
			return GetResource<Resource>(name);
		}

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::GetResource(const String &name)
		{

			Lock();

			auto it = m_resources.find(name);

			ResourcePointer r = (it != m_resources.end()
				? (it->second) :
				ResourcePointer());

			Unlock();

			return Resource::StaticCast<ResourceType>(r);

		}

	}

}