#include "Script.h"

using namespace mye::core;

Script::Script(void) :
	Resource(nullptr, "", nullptr)
{
}

Script::Script(mye::core::ResourceManager *rm, const mye::core::String &name) :
	Resource(rm, name, nullptr)
{
}


Script::~Script(void)
{
}

size_t Script::CalculateSizeImpl(void)
{
	return 1; // TODO: Calcolare rimensione script
}

void Script::Clear(void)
{
}