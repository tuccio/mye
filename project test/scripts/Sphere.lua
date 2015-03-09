local Sphere = { }

function Sphere:Init()

	--self.text2d.position = vec2i(8, 1120)
	--self.text2d.color    = vec4(0, 1, 0, 1)

end

function Sphere:Update()

--[[

	if (Input.keyboard:IsPressed(Keyboard.M)) then
		self.render.matrix = Math.ScaleMatrix4(vec3(1.1)) * self.render.matrix
	end
	
	if (Input.keyboard:IsPressed(Keyboard.N)) then
		self.render.matrix = Math.ScaleMatrix4(vec3(0.9)) * self.render.matrix
	end
	
	local text = nil
	
	if (self.rigidbody == nil) then
		text = "Sphere No rigid body attached"
	else
		text = "Sphere velocity: " .. tostring(self.rigidbody.velocity) .. " position: " .. tostring(self.rigidbody.position) .. " mass: " .. tostring(self.rigidbody.mass)
	end
	
	self.text2d.text = text
	
]]--

end

return Sphere