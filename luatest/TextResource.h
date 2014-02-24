#pragma once

#include <mye/core/ResourceManager.h>
#include <mye/core/Singleton.h>

#include <string>
#include <fstream>

class TextResource :
	public mye::core::Resource
{

public:

	TextResource(mye::core::ResourceManager *owner,
		const mye::core::String &name,
		mye::core::ManualResourceLoader *manual = nullptr) :
		Resource(owner, name, manual)
	{
	}

	const std::string& Get(void) const
	{
		return m_text;
	}

	void Set(const std::string &s)
	{
		m_text = s;
	}

protected:

	bool LoadImpl(void)
	{

		std::ifstream f(GetName().CString());
		m_text = std::string(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>());

		return true;

	}

	void UnloadImpl(void)
	{
		m_text.clear();
	}

	virtual size_t CalculateSizeImpl(void)
	{
		return m_text.size();
	}

private:

	std::string m_text;

};

class TextResourceHandle :
	public std::shared_ptr<TextResource>
{

public:

	TextResourceHandle(void) { }

	TextResourceHandle(TextResource *resource) :
		std::shared_ptr<TextResource>(resource) { }

	TextResourceHandle(const std::shared_ptr<TextResourceHandle> &resource) :
		std::shared_ptr<TextResource>()
	{
	}

	TextResourceHandle(mye::core::ResourceHandle resource) :
		std::shared_ptr<TextResource>(std::static_pointer_cast<TextResource>(resource))

	{
	}

};

class TextResourceManager :
	public mye::core::ResourceManager,
	public mye::core::Singleton<TextResourceManager>
{

public:

	TextResourceManager(void) :
		ResourceManager("Text")
	{

	}

private:

	mye::core::ResourceHandle CreateImpl(const mye::core::String &name,
		mye::core::ManualResourceLoader *manual,
		mye::core::Resource::ParametersList *params)
	{
		return mye::core::ResourceHandle(new TextResource(this, name));
	}

};