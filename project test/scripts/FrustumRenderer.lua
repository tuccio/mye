local FrustumRenderer = { }

function FrustumRenderer:Update()

	local frustum = self.camera:GetFrustum()
	Graphics:RenderFrustum(frustum, vec4(0, 1, 0, 1))

end

return FrustumRenderer