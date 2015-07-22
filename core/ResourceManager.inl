namespace mye
{

	namespace core
	{

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::CreateResource(const String & name,
		                                                              ManualResourceLoader * manual,
		                                                              const Parameters & params)
		{

			Lock();

			auto it = m_resources.find(name);

			if (it != m_resources.end())
			{
				return Resource::DynamicCast<ResourceType>(it->second);
			}

			Resource * r = CreateImpl(name, manual, params);
			r->SetParametersList(params);

			auto it2 = m_resources.emplace(std::make_pair(name, ResourcePointer(r)));
			
			Unlock();

			return Resource::StaticCast<ResourceType>(it2.first->second);

		}

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::GetResource(const String & name)
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