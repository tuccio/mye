namespace mye
{

	namespace core
	{

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::CreateResource(const String & name,
		                                                              ManualResourceLoader * manual,
		                                                              const Parameters & params)
		{

			std::shared_ptr<ResourceType> p;

			Lock();

			auto it = m_resources.find(name);

			if (it != m_resources.end())
			{
				p = Resource::DynamicCast<ResourceType>(it->second);
				p->SetParametersList(params);
			}
			else
			{

				Resource * r = CreateImpl(name, manual, params);
				r->SetParametersList(params);

				auto it2 = m_resources.emplace(std::make_pair(name, ResourcePointer(r)));

				p = Resource::StaticCast<ResourceType>(it2.first->second);

			}
			
			Unlock();

			return p;

		}

		template <typename ResourceType>
		std::shared_ptr<ResourceType> ResourceManager::GetResource(const String & name)
		{

			Lock();

			auto it = m_resources.find(name);

			ResourcePointer r = (it != m_resources.end() ? (it->second) : ResourcePointer());

			Unlock();

			return Resource::StaticCast<ResourceType>(r);

		}

	}

}