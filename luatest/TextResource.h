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
		const std::string &name,
		const std::string &group = MYE_DEFAULT_GROUP,
		mye::core::ManualResourceLoader *manual = NULL) :
		Resource(owner, name, group, manual)
	{
	}

	const std::string& Get(void) const
	{
		return _text;
	}

	void Set(const std::string &s)
	{
		_text = s;
	}

protected:

	bool LoadImpl(void)
	{

		std::ifstream f(GetName());
		_text = std::string(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>());

		return true;

	}

	void UnloadImpl(void)
	{
		_text.clear();
	}

	virtual size_t CalculateSizeImpl(void)
	{
		return _text.size();
	}

private:

	std::string _text;

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

	mye::core::ResourceHandle CreateImpl(const std::string &name,
		const std::string &group,
		mye::core::ManualResourceLoader *manual,
		mye::core::Resource::ParametersList *params)
	{
		return mye::core::ResourceHandle(new TextResource(this, name, group));
	}

};