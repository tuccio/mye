function Update()

	local x = self.transform.position
	
	if (x.x > 2) then
		self.acceleration = vec3(-2.8, 0, 0)
	elseif (x.x < -2) then
		self.acceleration = vec3(2.8, 0, 0)
	end
	
	local dv = self.acceleration * Time.Delta;
	self.velocity = self.velocity + dv;
	
	local dx = self.velocity * Time.Delta
	
	self.transform.position = x + dx

end