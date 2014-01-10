#include "IDGenerator.h"

using namespace mye::win;

IDGenerator::IDGenerator(void)
{
	m_nextID = 100;
}


IDGenerator::~IDGenerator(void)
{
}

IDGenerator::ID IDGenerator::Generate(void)
{
	return m_nextID++;
}

IDGenerator::ID IDGenerator::Generate(int n)
{
	int id = m_nextID;
	m_nextID += n;
	return id;
}