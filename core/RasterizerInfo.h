#pragma once

namespace mye
{

	namespace core
	{

		enum CullMode { NONE = 0, BACK, FRONT };

		struct RasterizerInfo
		{

			bool wireframe;
			CullMode cull;

		};

	};

};
