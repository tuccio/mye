local Camera = { }

function Camera:Init()
	
	self.speed       = 1
	
	self.camera.fovy = 90
	self.camera.far  = 100
	self.camera.near = 0.001
	
	Scene.camera = self.camera

end

function Camera:Update()
	
	self.camera.position    = self.transform.position
	self.camera.orientation = self.transform.orientation

end

function Camera:OnKeyboardKeyHold(key)

	local speed = self.speed
	local dx    = speed * Time.delta
	
	if key == KeyboardVK.W then
		self.transform.position = self.transform.position + self.camera.forward * dx
	elseif key == KeyboardVK.S then
		self.transform.position = self.transform.position - self.camera.forward * dx
	elseif key == KeyboardVK.D then
		self.transform.position = self.transform.position + self.camera.right * dx
	elseif key == KeyboardVK.A then
		self.transform.position = self.transform.position - self.camera.right * dx
	elseif key == KeyboardVK.E then
		self.transform.position = self.transform.position + self.camera.up * dx
	elseif key == KeyboardVK.Q then
		self.transform.position = self.transform.position - self.camera.up * dx
	end
	
end

function Camera:OnMouseMove(from, to)

	if Input.mouse:IsPressed(MouseVK.Right) then
	
		local delta = to - from
		
		local theta = 180 * math.atan(delta.x) / math.pi
		local phi   = 180 * math.atan(delta.y) / math.pi
		
		local orientation = quaternion(self.transform.orientation)
		
		local r1 = quaternion(vec3(0, 1, 0), theta)
		local r2 = quaternion(vec3(1, 0, 0), phi)
		
		local newOrientation = (r1 * orientation * r2):Normalize()
		
		self.transform.orientation = newOrientation
		
	end

end

return Camera