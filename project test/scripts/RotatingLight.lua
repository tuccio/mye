local RotatingLight = { }

function RotatingLight:Update()

	--Halt()
	
	print(self.t)
	local t = self.t + Time.delta * self.omega
	self.t = t
	
	local r = self.r

	local x = r * math.cos(t)
	local y = r * math.sin(t)
	
	local p = vec4(x, y, 0, 1)
	
	self.transform.position = self.motionTransform * p
	print(self.transform.position)

end

return RotatingLight