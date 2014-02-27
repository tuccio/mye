#include <mye/core/String.h>
#include <mye/core/Stream.h>
#include <mye/core/EntityParser.h>
#include <mye/core/ResourceTypeManager.h>

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>

using namespace mye::core;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

int main(int argc, char *argv[])
{

	ResourceTypeManager rtm;
	EntityTemplateManager etm;

	ResourceHandle sphereRH = etm.CreateResource("Sphere");
	
	if (sphereRH->Load())
	{

		for (Component *c : *sphereRH.Cast<EntityTemplate>())
		{

			StandardOutput << static_cast<int>(c->GetComponentType()) << NewLine;

		}

	}
	else
	{

		StandardOutput << "Parse error" << NewLine;

	}

	system("pause");

	return 0;

}