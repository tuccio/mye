#pragma once

#include "AlignedAllocator.h"

#include "Component.h"
#include "Material.h"
#include "Mesh.h"
#include "GPUBuffer.h"
#include "Texture.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class RenderComponent :
			public Component
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			RenderComponent(void);
			RenderComponent(MeshPointer mesh, MaterialPointer material, const mye::math::Matrix4 & modelMatrix);
			~RenderComponent(void);

			bool IsVisible(void) const;
			void SetVisible(bool visible);

			MeshPointer GetMesh(void);
			void SetMesh(const MeshPointer & mesh);

			MaterialPointer GetMaterial(void) const;
			void SetMaterial(const MaterialPointer & material);

			const mye::math::AABB & GetBounds(void) const;
			void SetBounds(const mye::math::AABB & bounds);

			const mye::math::Matrix4 & GetModelMatrix(void) const;
			void SetModelMatrix(const mye::math::Matrix4 & matrix);

			TexturePointer GetDiffuseTexture(void) const;
			void SetDiffuseTexture(const TexturePointer & diffuseTex);

			TexturePointer GetSpecularTexture(void) const;
			void SetSpecularTexture(const TexturePointer & specularTex);

			TexturePointer GetNormalHeightMap(void) const;
			void SetNormalHeightMap(const TexturePointer & heightMap);

			RenderComponent * Clone(void) const;

			GPUBufferPointer GetGPUBuffer(void) const;
			
		private:

			bool m_visible;
			
			MeshPointer         m_mesh;
			GPUBufferPointer    m_gpuBuffer;

			MaterialPointer     m_material;

			TexturePointer      m_diffuseTex;
			TexturePointer      m_specularTex;
			TexturePointer      m_normalHeightMap;
			
			mye::math::AABB     m_bounds;
			mye::math::Matrix4  m_modelMatrix;


			
		};

	}

}

