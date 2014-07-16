function Init()
	self.transform.position = vec3(10, 0, 6)
	self.velocity = vec3(-2, 2, 0)
	self.text2d.color = vec4(1, 0, 1, 1)
	self.text2d.position = vec2i(1050, 8)
end

function Update()

	local v = 0
	
	if (Input.keyboard:IsPressed(Keyboard.L)) then
		self.moving = not self.moving
	end
	
	if (self.moving) then
		v = 2
	end

	if (self.transform.position.x >= 10) then
		self.velocity.x = - v
	elseif (self.transform.position.x <= -10) then
		self.velocity.x = v
	end
	
	if (self.transform.position.y >= 10) then
		self.velocity.y = - v
	elseif (self.transform.position.y <= -10) then
		self.velocity.y = v
	end

	if (self.moving) then
		self.transform.position = self.transform.position + self.velocity * Time.delta
	end
	
	self.text2d.text = "Light position: " .. tostring(self.transform.position) .. "\nvelocity: " .. tostring(self.velocity) .. "\nmoving: " .. tostring(self.moving)

end

function Stop()
	self.moving = false
end