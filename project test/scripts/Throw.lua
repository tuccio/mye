local Throw = { }

function Throw:Init()
	print('sup')
end

function Throw:OnMouseKeyPress(key)

	local camera = Scene.camera
	local ray = camera:RayCast(Input.mouse.position)
	
	local intersection = Scene:Pick(ray)
	
	if intersection.hObj ~= self.selected then
		self.hSelected = intersection.hObj
	end
	
	self.text2d.text = tostring(self.hSelected)

end

function Throw:OnMouseKeyRelease(key, t)
		
	if self.hSelected:Exists() and self.hSelected.rigidbody ~= nil then
		local delta = Input.mouse.delta
		--local f = 1000 * t * (delta.x * Scene.camera.right + delta.y * Scene.camera.up)
		--print(f)
		local f = 50 * (delta.x * Scene.camera.right - 2 * delta.y * Scene.camera.up) / t
		print(f)
		--self.hSelected.rigidbody:ApplyImpulse(f)
		self.hSelected.rigidbody.velocity = f
	end
	
	self.hSelected:Clear()
	
end

return Throw