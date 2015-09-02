#ifndef __MYE_CAMERA_TRANSFORM__
#define __MYE_CAMERA_TRANSFORM__

#include "register_slots.hlsli"

cbuffer cbCameraTransform : register(__MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM)
{

	struct Camera
	{

		float4x4 view;
		float4x4 viewProj;
		float4x4 invViewProj;

		float3   position;

		float    near;
		float    far;

		float    fovy;
		float    ratio;

	} g_camera;
};

#endif