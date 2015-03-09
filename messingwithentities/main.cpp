#include <mye/core/Entity.h>
#include <mye/core/EntityManager.h>
#include <mye/core/EntityParser.h>
#include <mye/core/EntityInstantiation.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/VariableTypes.h>

#include <boost/variant.hpp>

#include <fstream>

using namespace mye::core;

struct visitor : public boost::static_visitor<void>
{

	template <typename T>
	void operator() (T v) const
	{
		std::cout << std::boolalpha << v << " ";
	}

	void operator() (std::nullptr_t) const
	{
		std::cout << "null ";
	}

};

void Visit(const entity::InitValue & x)
{

	/*if (x.initializers.size())
	{

		std::cout << "Constructor (" << (int)x.constructible << ") { ";

		for (auto &iv : x.initializers)
		{
			Visit(iv);
		}

		std::cout << "} ";

	}
	else
	{
		boost::apply_visitor(visitor(), x.value);
	}*/

	if (x.constructible == entity::Constructible::VEC3)
	{
		mye::math::Vector3 v = mye::core::entity::ObjectInstantiator<mye::math::Vector3>::Instantiate(x);
		std::cout << v.x() << v.y() << v.z() << std::endl;
	}

}

struct StatementVisitor : boost::static_visitor<void>
{

	void operator() (const entity::VariableDefinition &vd) const
	{
		std::cout << "Variable " << vd.name << " (" << (int) vd.type << ") ";
		
		if (vd.initializer) Visit(vd.initializer.get());

	}

	void operator() (const entity::ComponentDefinition &cd) const
	{
		std::cout << "Component " << (int) cd.component << " ";
		if (cd.initializer) Visit(cd.initializer.get());
	}

};

int main(int argc, char *argv[])
{

	typedef std::string::const_iterator Iterator;

	ResourceTypeManager trm;
	EntityManager       em;

	/*EntityPointer entity = trm.CreateResource<Entity>("Entity", "Toy");
	
	entity->Load();*/

	//std::ifstream f("test");

	//std::string code = std::string(
	//	std::istreambuf_iterator<char>(f),
	//	std::istreambuf_iterator<char>());

	////entity::Init x;
	////entity::VariableDefinition x;
	//entity::EntityDefinition x;

	//auto it = code.begin();

	//if (!entity::qi::phrase_parse(it, code.end(), entity::EntityParser<Iterator>(), entity::SkipGrammar<Iterator>(), x) ||
	//	it != code.end())
	//{
	//	std::cout << "failed" << std::endl;
	//}
	//else
	//{
	//	std::cout << "succeeded" << std::endl;

	//	std::cout << x.name << std::endl;

	//	for (auto & s : x.statements)
	//	{
	//		boost::apply_visitor(StatementVisitor(), s);
	//		std::cout << std::endl;
	//	}

	//}	

	EntityPointer e = trm.CreateResource<Entity>("Entity", "Test");

	if (!e->Load())
	{
		std::cout << "Fail" << std::endl;
	}

	else
	{

		GameObject object;

		e->Instantiate(&object);

	}

	return 0;

}