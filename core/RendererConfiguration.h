#pragma once

#include <mye/math/Math.h>

#include <algorithm>
#include <vector>

#define __MYE_RENDERERCONFIGURATION_PROPERTY(Variable, Property, Member) \
	inline void Set ## Property (const decltype(Member) & value) { Member = value; NotifyVariableChange(Variable); }\
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
			VSMMINVARIANCE,
			VSMMINBLEEDING
		};

		class RendererConfiguration;

		struct RendererConfigurationListener
		{
			virtual void OnConfigurationChange(RendererVariable variable, RendererConfiguration * configuration) { }
		};

		class RendererConfiguration
		{

		protected:

			mye::math::Vector2i m_resolution;
			mye::math::Real     m_gamma;
			int                 m_shadowMapResolution;

			mye::math::Real     m_vsmMinVariance;
			mye::math::Real     m_vsmMinBleeding;

		public:

			RendererConfiguration(void) :
				m_resolution(mye::math::Vector2i(1280, 720)),
				m_gamma(1.8f),
				m_shadowMapResolution(1024),
				m_vsmMinVariance(mye::math::Epsilon()),
				m_vsmMinBleeding(0.2f)
			{

			}

			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::RESOLUTION,          ScreenResolution,    m_resolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::GAMMA,               Gamma,               m_gamma)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::SHADOWMAPRESOLUTION, ShadowMapResolution, m_shadowMapResolution)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINVARIANCE,      VSMMinVariance,      m_vsmMinVariance)
			__MYE_RENDERERCONFIGURATION_PROPERTY(RendererVariable::VSMMINBLEEDING,      VSMMinBleeding,      m_vsmMinBleeding)

			void AddListener(RendererConfigurationListener * listener)
			{
				m_listeners.push_back(listener);
			}

			void RemoveListener(RendererConfigurationListener * listener)
			{
				std::remove(m_listeners.begin(), m_listeners.end(), listener);
			}

		private:

			void NotifyVariableChange(RendererVariable variable)
			{
				for (auto listener : m_listeners)
				{
					listener->OnConfigurationChange(variable, this);
				}
			}

			std::vector<RendererConfigurationListener*> m_listeners;

		};

	}

}