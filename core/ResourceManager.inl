namespace mye
{

	namespace core
	{

		boost::shared_ptr<Resource> ResourceManager::CreateResource(const String &name,
			ManualResourceLoader *manual,
			const Resource::ParametersList &params)
		{
			return CreateResource<Resource>(name, manual, params);
		}

		template <typename ResourceType>
		boost::shared_ptr<ResourceType> ResourceManager::CreateResource(const String &name,
			ManualResourceLoader *manual,
			const Resource::ParametersList &params)
		{

			Lock();

			auto it = m_resources.find(name);

			if (it != m_resources.end())
			{
				return boost::static_pointer_cast<ResourceType>(it->second);
			}

			Resource *r = CreateImpl(name, manual, params);
			r->SetParametersList(params);

			auto it2 = m_resources.insert(std::pair<String, ResourcePointer>(name, ResourcePointer(r)));
			
			Unlock();

			return boost::static_pointer_cast<ResourceType>(it2.first->second);

		}

		boost::shared_ptr<Resource> ResourceManager::GetResource(const String &name)
		{
			return GetResource<Resource>(name);
		}

		template <typename ResourceType>
		boost::shared_ptr<ResourceType> ResourceManager::GetResource(const String &name)
		{

			Lock();

			auto it = m_resources.find(name);

			ResourcePointer r = (it != m_resources.end()
				? (it->second) :
				ResourcePointer());

			Unlock();

			return boost::static_pointer_cast<ResourceType>(r);

		}

	}

}