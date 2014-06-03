#include "GraphicsModule.h"

using namespace mye::core;

GraphicsModule::GraphicsModule(void) :
	m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
	m_mainWindowPointer = nullptr;
}

const mye::math::Vector4f& GraphicsModule::GetClearColor(void) const
{
	return m_clearColor;
}

void GraphicsModule::SetClearColor(const mye::math::Vector4f &color)
{
	m_clearColor = color;
}