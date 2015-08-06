#pragma once

#include <mye/math/Math.h>

#include <algorithm>
#include <vector>

#include "EventManager.h"

#define __MYE_RENDERERCONFIGURATION_PROPERTY(Variable, Property, Member) \
	inline void Set ## Property (const decltype(Member) & value) { Member = value; MYE_EVENT_MANAGER_TRIGGER(RenderVariableChange, Variable, this); }\
	inline auto Get ## Property (void) -> decltype(Member) const { return Member; }

namespace mye
{

	namespace core
	{

		enum class RendererVariable
		{
			RESOLUTION,
			GAMMA,
			SHADOWMAPRESOLUTION,
			SHADOWMAPBIAS,
			SHADOWMAPSLOPESCALEDBIAS,
			SHADOWMAPNORMALOFFSETBIAS,
			VSMMINVARIANCE,
			VSMMINBLEEDING,
			PCFENABLED,
			PCFKERNEL,
			CSMSPLITS,
			CSMDEBUG
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

		protected:

			mye::math::Vector2i m_resolution;
			mye::math::Real     m_gamma;

			int                 m_shadowMapResolution;
			mye::math::Real     m_shadowMapBias;
			mye::math::Real     m_shadowMapSlopeScaledBias;
			mye::math::Real     m_shadowMapNormalOffsetBias;

			mye::math::Real     m_vsmMinVariance;
			mye::math::Real     m_vsmMinBleeding;

			unsigned int        m_csmSplits;
			unsigned int        m_pcfKernel;

			bool                m_csmDebug;
			bool                m_pcfEnabled;

		public:

			RendererConfiguration(void) :
				m_resolution(mye::math::Vector2i(1280, 720)),
				m_gamma(1.8f),
				m_shadowMapResolution(1024),
				m_shadowMapBias(.0f),
				m_shadowMapSlopeScaledBias(.0f),
				m_shadowMapNormalOffsetBias(.0f),
				m_vsmMinVariance(mye::math::Epsilon()),
				m_vsmMinBleeding(.2f),
				m_pcfEnabled(false),
				m_csmDebug(false),
				m_csmSplits(3),
				m_pcfKernel(3)
			{
			}
																							 
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::RESOLUTION,                ScreenResolution,          m_resolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::GAMMA,                     Gamma,                     m_gamma)
																							 				      	     
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPRESOLUTION,       ShadowMapResolution,       m_shadowMapResolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPBIAS,             ShadowMapBias,             m_shadowMapBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPSLOPESCALEDBIAS,  ShadowMapSlopeScaledBias,  m_shadowMapSlopeScaledBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPNORMALOFFSETBIAS, ShadowMapNormalOffsetBias, m_shadowMapNormalOffsetBias)

			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINVARIANCE,            VSMMinVariance,            m_vsmMinVariance)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINBLEEDING,            VSMMinBleeding,            m_vsmMinBleeding)
																						      				             
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::CSMSPLITS,                 CSMSplits,                 m_csmSplits)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::CSMDEBUG,                  CSMDebug,                  m_csmDebug)
																						      				             
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::PCFENABLED,                PCFEnabled,                m_pcfEnabled)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::PCFKERNEL,                 PCFKernel,                 m_pcfKernel)

		};

		

	}

}