#include "CameraComponent.h"

using namespace mye::core;

CameraComponent::CameraComponent(void) :
	Component(ComponentTypes::CAMERA, "camera")
{
}


CameraComponent::~CameraComponent(void)
{
}

CameraComponent* CameraComponent::Clone(void) const
{
	return new CameraComponent(*this);
}