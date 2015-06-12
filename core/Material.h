#pragma once

#include <mye/math/Math.h>

#include "AlignedAllocator.h"
#include "Resource.h"

namespace mye
{

	namespace core
	{

		class Material :
			public Resource
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			Material(ResourceManager * owner,
					 const String & name,
					 ManualResourceLoader * manual);

			inline const mye::math::Vector4 & GetDiffuseColor(void) const
			{
				return m_diffuseColor;
			}

			inline void SetDiffuseColor(const mye::math::Vector4 & color)
			{
				m_diffuseColor = color;
			}

			inline mye::math::Vector4 * GetDiffuseColorPointer(void)
			{
				return &m_diffuseColor;
			}

			inline const mye::math::Vector4 & GetSpecularColor(void) const
			{
				return m_specularColor;
			}

			inline void SetSpecularColor(const mye::math::Vector4 & color)
			{
				m_specularColor = color;
			}

			inline mye::math::Vector4 * GetSpecularColorPointer(void)
			{
				return &m_specularColor;
			}

			inline mye::math::Real GetSpecular(void) const
			{
				return m_specular;
			}

			inline void SetSpecular(const mye::math::Real & specular)
			{
				m_specular = specular;
			}

			inline mye::math::Real * GetSpecularPointer(void)
			{
				return &m_specular;
			}

			inline mye::math::Real GetRoughness(void) const
			{
				return m_roughness;
			}

			inline void SetRoughness(const mye::math::Real & roughness)
			{
				m_roughness = roughness;
			}

			inline mye::math::Real * GetRoughnessPointer(void)
			{
				return &m_roughness;
			}

			inline mye::math::Real GetMetallic(void) const
			{
				return m_metallic;
			}

			inline void SetMetallic(const mye::math::Real & metallic)
			{
				m_metallic = metallic;
			}

			inline mye::math::Real * GetMetallicPointer(void)
			{
				return &m_metallic;
			}

		protected:

			virtual bool LoadImpl(void);
			virtual void UnloadImpl(void);

			virtual size_t CalculateSizeImpl(void);

		private:

			mye::math::Vector4 m_diffuseColor;
			mye::math::Vector4 m_specularColor;

			mye::math::Real m_specular;
			mye::math::Real m_roughness;
			mye::math::Real m_metallic;

		};

		typedef std::shared_ptr<Material> MaterialPointer;

	}
}
