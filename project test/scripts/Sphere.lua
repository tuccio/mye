function Update(self, dt)

	local x = self.transform.position
	
	if (x.x > 6 or x.x < -6) then
		self.velocity = - self.velocity
	end
	
	local dx = self.velocity * dt
	
	self.transform.position = x + dx

end