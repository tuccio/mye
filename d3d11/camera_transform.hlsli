#ifndef __MYE_CAMERA_TRANSFORM__
#define __MYE_CAMERA_TRANSFORM__

#include "register_slots.hlsli"

cbuffer cbCameraTransform : register(__MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM)
{
	float4x4 g_cameraView;
	float4x4 g_cameraViewProj;
	float4x4 g_cameraInvViewProj;
};

#endif