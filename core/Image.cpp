#include "Image.h"
#include "Logger.h"

#include <fstream>

#include <IL/ilu.h>

using namespace mye::core;

Image::Image(ResourceManager      * owner,
			 const String         & name,
			 ManualResourceLoader * manual) :
	Resource(owner, name, manual),
	m_id(IL_INVALID_VALUE)
{
}

Image::Image(const Image & image)
{
	ilGenImages(1, &m_id);
	ilBindImage(m_id);
	ilCopyImage(image.m_id);
}

Image::Image(Image && image)
{
	m_id = image.m_id;
	image.m_id = IL_INVALID_VALUE;
}

Image::~Image(void)
{
}

void Image::Destroy(void)
{

	if (m_id != IL_INVALID_VALUE)
	{
		ilDeleteImage(m_id);
		m_id = IL_INVALID_VALUE;
	}

}

bool Image::LoadImpl(void)
{

	bool loaded = false;

	ilGenImages(1, &m_id);

	if (m_id != IL_INVALID_VALUE)
	{

		ilBindImage(m_id);

		std::ifstream f(m_name.CString());

		if (f)
		{
			
			if (ilLoadImage(m_name.CString()) &&
				ilConvertImage(IL_RGBA, IL_FLOAT))
			{
				loaded = true;
			}
			else
			{
				Logger::LogErrorOptional("Image Load", "Failed to load image " + m_name + ".");
			}

		}
		else
		{
			Logger::LogErrorOptional("Image Load", "Failed to open file " + m_name + ".");
		}

	}

	return loaded;

}

void Image::UnloadImpl(void)
{

	Destroy();

}

size_t Image::CalculateSizeImpl(void)
{
	ilBindImage(m_id);
	return ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
}

int Image::GetWidth(void) const
{
	ilBindImage(m_id);
	return ilGetInteger(IL_IMAGE_WIDTH);
}

int Image::GetHeight(void) const
{
	ilBindImage(m_id);
	return ilGetInteger(IL_IMAGE_HEIGHT);
}

const void * Image::GetData(void) const
{
	ilBindImage(m_id);
	return (const void *) ilGetData();
}

Image Image::Scale(float scale) const
{
	
	Image image(*this);

	ilBindImage(image.m_id);

	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);

	iluImageParameter(ILU_FILTER, ILU_SCALE_BOX);
	iluScale(w * scale, h * scale, 1);

	return image;

}