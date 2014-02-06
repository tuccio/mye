#include "GraphicsModule.h"

using namespace mye::core;

GraphicsModule::GraphicsModule(void) :
	m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
	m_mainWindowPointer = nullptr;
}

const ColorRGBA& GraphicsModule::GetClearColor(void) const
{
	return m_clearColor;
}

void GraphicsModule::SetClearColor(const ColorRGBA &color)
{
	m_clearColor = color;
}