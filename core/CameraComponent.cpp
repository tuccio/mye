#include "CameraComponent.h"

using namespace mye::core;

CameraComponent::CameraComponent(void) :
	Component(ComponentTypes::CAMERA, "camera")
{
}

CameraComponent::CameraComponent(const Camera &camera) :
	Component(ComponentTypes::CAMERA, "camera"),
	Camera(camera)
{
}

CameraComponent::~CameraComponent(void)
{
}

CameraComponent * CameraComponent::Clone(void) const
{
	return new CameraComponent(*this);
}

Camera CameraComponent::GetCamera(void) const
{
	return *this;
}

void CameraComponent::SetCamera(const Camera & camera)
{
	static_cast<Camera&>(*this) = camera;
}