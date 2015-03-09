#pragma once

namespace mye
{

	namespace core
	{

		template <int ... N>
		struct IntegerSequence
		{

		};

		template <int Max, int ... Sequence>
		struct IntegerSequenceGenerator :
			IntegerSequenceGenerator<Max - 1, Max - 1, Sequence ... >
		{

		};

		template <int ... Sequence>
		struct IntegerSequenceGenerator<0, Sequence ...>
		{
			typedef IntegerSequence<Sequence ...> type;
		};

	}

}