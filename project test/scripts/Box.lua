function Init()

	Physics.gravity = vec3(0, 0, 0)
	
	self.id = tonumber(self.name:sub(4))
	
	local position = vec3(0, -1.17, 8.73)
	local mass     = 1
	
	if (self.id == 1) then
		position.x = -4
	else
		position.x = 4
		mass       = 0.25
	end
	
	local box = ResourceTypeManager:CreateResource(
		"BulletCollisionShape",
		"BOX_SHAPE",
		nil,
		{ ["type"] = "box", ["halfextents"] = "1 1 1" })
	
	self:AddComponent(RigidBodyComponent(box, mass))
	
	self.rigidbody.position = position
	self.rigidbody.orientation = quaternion(vec3(0, 1, 0), 45)

end

function Update()

	--[[

	local id = self.id
	
	if (self.id == 1) then

		if (Input.keyboard:IsPressed(Keyboard.D)) then
			self.rigidbody.velocity = self.rigidbody.velocity + vec3(1, 0, 0)
		end
	
		if (Input.keyboard:IsPressed(Keyboard.A)) then
			self.rigidbody.velocity = self.rigidbody.velocity - vec3(1, 0, 0)
		end
	
		if (Input.keyboard:IsPressed(Keyboard.S)) then
			self.rigidbody.velocity = vec3(0)
		end
	
	else
	
		if (Input.keyboard:IsPressed(Keyboard.Q)) then
			self.rigidbody.velocity = self.rigidbody.velocity - vec3(1, 0, 0)
		end
	
		if (Input.keyboard:IsPressed(Keyboard.E)) then
			self.rigidbody.velocity = self.rigidbody.velocity + vec3(1, 0, 0)
		end
	
		if (Input.keyboard:IsPressed(Keyboard.S)) then
			self.rigidbody.velocity = vec3(0)
		end
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.G) and self.id == 1) then
		if (Physics.gravity.y == 0) then
			Physics.gravity = vec3(0, -10, 0)
		elseif (self.id == 1) then
			Physics.gravity = vec3(0)
		end
	end
	
	if (Input.keyboard:IsPressed(Keyboard.F)) then
		Windows.MessageBox("Object " .. tostring(id), "Position: " .. tostring(self.rigidbody.position) .. "\nVelocity: " .. tostring(self.rigidbody.velocity))
	end
	
	]]--

end

function Finalize()

end