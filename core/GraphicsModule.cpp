#include "GraphicsModule.h"

using namespace mye::core;

GraphicsModule::GraphicsModule(void) :
	m_clearColor(0.0f, 0.0f, 0.0f, 1.0f),
	m_vsync(false)
{
	m_mainWindowPointer = nullptr;
}

const mye::math::Vector4& GraphicsModule::GetClearColor(void) const
{
	return m_clearColor;
}

void GraphicsModule::SetClearColor(const mye::math::Vector4 &color)
{
	m_clearColor = color;
}

bool GraphicsModule::GetVSync(void) const
{
	return m_vsync;
}

void GraphicsModule::SetVSync(bool vsync)
{
	m_vsync = vsync;
}