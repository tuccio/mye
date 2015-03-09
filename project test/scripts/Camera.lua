local Camera = { }

function Camera:Init()
	
	self.camera.fovy = 60
	self.camera.far  = 100
	
	Scene.camera = self.camera

end

function Camera:Update()
	
	self.camera.position    = self.transform.position
	self.camera.orientation = self.transform.orientation
	
	-- local speed = 0.005
	
	-- if (Input.keyboard:IsPressed(KeyboardVK.W)) then
		-- self.transform.position = self.transform.position + self.camera.forward * speed
	-- elseif (Input.keyboard:IsPressed(KeyboardVK.S)) then
		-- self.transform.position = self.transform.position - self.camera.forward * speed
	-- elseif (Input.keyboard:IsPressed(KeyboardVK.D)) then
		-- self.transform.position = self.transform.position + self.camera.right * speed
	-- elseif (Input.keyboard:IsPressed(KeyboardVK.A)) then
		-- self.transform.position = self.transform.position - self.camera.right * speed
	-- elseif (Input.keyboard:IsPressed(KeyboardVK.E)) then
		-- self.transform.position = self.transform.position + self.camera.up * speed
	-- elseif (Input.keyboard:IsPressed(KeyboardVK.Q)) then
		-- self.transform.position = self.transform.position - self.camera.up * speed
	-- end

end

function Camera:OnKeyboardKeyHold(key)

	local speed = 0.005
	
	if key == KeyboardVK.W then
		self.transform.position = self.transform.position + self.camera.forward * speed
	elseif key == KeyboardVK.S then
		self.transform.position = self.transform.position - self.camera.forward * speed
	elseif key == KeyboardVK.D then
		self.transform.position = self.transform.position + self.camera.right * speed
	elseif key == KeyboardVK.A then
		self.transform.position = self.transform.position - self.camera.right * speed
	elseif key == KeyboardVK.E then
		self.transform.position = self.transform.position + self.camera.up * speed
	elseif key == KeyboardVK.Q then
		self.transform.position = self.transform.position - self.camera.up * speed
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
		
		self.transform.orientation = r1 * orientation * r2
		
	end

end

return Camera