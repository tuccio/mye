#pragma once

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <algorithm>
#include <vector>

#include "EventManager.h"

#define __MYE_RENDERERCONFIGURATION_PROPERTY(MemberType, Variable, Property, DefaultValue) \
	private: MemberType m_p ## Property = DefaultValue; public: \
	inline void Set ## Property (const MemberType & value) { m_p ## Property = value; MYE_EVENT_MANAGER_TRIGGER(RenderVariableChange, Variable, this); }\
	inline MemberType Get ## Property (void) const { return m_p ## Property; }

namespace mye
{

	namespace core
	{

		enum class RendererVariable
		{
			RESOLUTION,
			GAMMA,
			MSAA,
			SHADOWMAPRESOLUTION,
			SHADOWMAPBIAS,
			SHADOWMAPSLOPESCALEDBIAS,
			SHADOWMAPNORMALOFFSETBIAS,
			VSMMINVARIANCE,
			VSMMINBLEEDING,
			VSMENABLED,
			VSMEXPONENTIAL,
			ESMPOSITIVEEXPONENT,
			ESMNEGATIVEEXPONENT,
			PCFENABLED,
			PCFKERNEL,
			CSMSPLITS,
			CSMDEBUG,
			LPVENABLED,
			LPVAABB,
			LPVRSMSAMPLES,
			LPVRESOLUTION,
			LPVITERATIONS,
			LPVGEOMETRYINJECTIONBIAS,
			LPVFLUXINJECTIONBIAS,
			LPVATTENUATION,
			PPBLOOMTHRESHOLD,
			PPBLOOMPOWER
		};

		class RendererConfiguration;

		struct RenderVariableChange :
			IEvent
		{

			RenderVariableChange(RendererVariable variable, RendererConfiguration * configuration) :
				IEvent(EventType::RENDERER_VARIABLE_CHANGE),
				variable(variable),
				configuration(configuration) { }

			RendererVariable        variable;
			RendererConfiguration * configuration;

		};

		class RendererConfiguration
		{
																							 
			__MYE_RENDERERCONFIGURATION_PROPERTY(mye::math::Vector2i, RendererVariable::RESOLUTION,                ScreenResolution,          mye::math::Vector2i(1280, 720))
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::GAMMA,                     Gamma,                     2.2f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::MSAA,                      MSAA,                      0)

			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::SHADOWMAPRESOLUTION,       ShadowMapResolution,       1024)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::SHADOWMAPBIAS,             ShadowMapBias,             0.f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::SHADOWMAPSLOPESCALEDBIAS,  ShadowMapSlopeScaledBias,  0.f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::SHADOWMAPNORMALOFFSETBIAS, ShadowMapNormalOffsetBias, 0.f)
													                 
			__MYE_RENDERERCONFIGURATION_PROPERTY(bool,                RendererVariable::VSMENABLED,                VSMEnabled,                false)
			__MYE_RENDERERCONFIGURATION_PROPERTY(bool,                RendererVariable::VSMEXPONENTIAL,            VSMExponential,            false)

			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::VSMMINVARIANCE,            VSMMinVariance,            .00001f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::VSMMINBLEEDING,            VSMMinBleeding,            .5f)

			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::ESMPOSITIVEEXPONENT,       ESMPositiveExponent,       30.f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::ESMNEGATIVEEXPONENT,       ESMNegativeExponent,       5.f)
													                 
			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::CSMSPLITS,                 CSMSplits,                 1)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::CSMDEBUG,                  CSMDebug,                  false)
													                 
			__MYE_RENDERERCONFIGURATION_PROPERTY(bool,                RendererVariable::PCFENABLED,                PCFEnabled,                false)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::PCFKERNEL,                 PCFKernel,                 2)

			__MYE_RENDERERCONFIGURATION_PROPERTY(bool,                RendererVariable::LPVENABLED,                LPVEnabled,                false)
			__MYE_RENDERERCONFIGURATION_PROPERTY(mye::math::AABB,     RendererVariable::LPVAABB,                   LPVAABB,                   mye::math::AABB::FromMinMax(-1.f, 1.f))
			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::LPVRSMSAMPLES,             LPVRSMSamples,             256)
			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::LPVRESOLUTION,             LPVResolution,             32)
			__MYE_RENDERERCONFIGURATION_PROPERTY(int,                 RendererVariable::LPVITERATIONS,             LPVIterations,             12)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::LPVGEOMETRYINJECTIONBIAS,  LPVGeometryInjectionBias,  1.f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::LPVFLUXINJECTIONBIAS,      LPVFluxInjectionBias,      .5f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::LPVATTENUATION,            LPVAttenuation,            1.f)

			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::PPBLOOMTHRESHOLD,          PPBloomThreshold,          5.f)
			__MYE_RENDERERCONFIGURATION_PROPERTY(float,               RendererVariable::PPBLOOMPOWER,              PPBloomPower,              1.f)

		};

		

	}

}