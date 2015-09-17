#pragma once

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

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
			VSMENABLED,
			VSMEXPONENTIAL,
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
			LPVATTENUATION
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

			bool                m_vsmEnabled;
			bool                m_vsmExponential;
			mye::math::Real     m_vsmMinVariance;
			mye::math::Real     m_vsmMinBleeding;

			bool                m_lpvEnabled;
			mye::math::AABB     m_lpvAABB;

			mye::math::Real     m_lpvRSMSamples;
			mye::math::Real     m_lpvResolution;
			unsigned int        m_lpvIterations;
			mye::math::Real     m_lpvGeometryInjectionBias;
			mye::math::Real     m_lpvFluxInjectionBias;
			mye::math::Real     m_lpvAttenuation;

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
				m_vsmEnabled(false),
				m_vsmExponential(false),
				m_vsmMinVariance(.00001f),
				m_vsmMinBleeding(.5f),
				m_pcfEnabled(false),
				m_csmDebug(false),
				m_csmSplits(3),
				m_pcfKernel(3),
				m_lpvEnabled(false),
				m_lpvAABB(mye::math::AABB::FromMinMax(-1.f, 1.f)),
				m_lpvRSMSamples(256),
				m_lpvResolution(32),
				m_lpvIterations(8),
				m_lpvGeometryInjectionBias(0.f),
				m_lpvFluxInjectionBias(.5f),
				m_lpvAttenuation(1.f)
			{
			}
																							 
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::RESOLUTION,                ScreenResolution,          m_resolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::GAMMA,                     Gamma,                     m_gamma)
																							 				      	     
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPRESOLUTION,       ShadowMapResolution,       m_shadowMapResolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPBIAS,             ShadowMapBias,             m_shadowMapBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPSLOPESCALEDBIAS,  ShadowMapSlopeScaledBias,  m_shadowMapSlopeScaledBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPNORMALOFFSETBIAS, ShadowMapNormalOffsetBias, m_shadowMapNormalOffsetBias)

			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMENABLED,                VSMEnabled,                m_vsmEnabled)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMEXPONENTIAL,            VSMExponential,            m_vsmExponential)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINVARIANCE,            VSMMinVariance,            m_vsmMinVariance)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINBLEEDING,            VSMMinBleeding,            m_vsmMinBleeding)
																						      				             
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::CSMSPLITS,                 CSMSplits,                 m_csmSplits)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::CSMDEBUG,                  CSMDebug,                  m_csmDebug)
																						      				             
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::PCFENABLED,                PCFEnabled,                m_pcfEnabled)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::PCFKERNEL,                 PCFKernel,                 m_pcfKernel)

			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVENABLED,                LPVEnabled,                m_lpvEnabled)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVAABB,                   LPVAABB,                   m_lpvAABB)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVRSMSAMPLES,             LPVRSMSamples,             m_lpvRSMSamples)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVRESOLUTION,             LPVResolution,             m_lpvResolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVITERATIONS,             LPVIterations,             m_lpvIterations)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVGEOMETRYINJECTIONBIAS,  LPVGeometryInjectionBias,  m_lpvGeometryInjectionBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVFLUXINJECTIONBIAS,      LPVFluxInjectionBias,      m_lpvFluxInjectionBias)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::LPVATTENUATION,            LPVAttenuation,            m_lpvAttenuation)

		};

		

	}

}