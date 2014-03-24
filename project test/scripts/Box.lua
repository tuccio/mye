function Init()

	Physics.gravity = vec3(0, 0, 0)
	
	self.id = tonumber(self.name:sub(4))
	
	local position = vec3(0, -1.17, 8.73)
	
	if (self.id == 1) then
		position.x = -4
	else
		position.x = 4
	end
	
	local box = ResourceTypeManager:CreateResource(
		"BulletCollisionShape",
		"BOX_SHAPE",
		nil,
		{ ["type"] = "box", ["halfextents"] = "2 2 2" })
	
	self:AddComponent(
		RigidBodyComponent(
			box,
			1,
			position,
			quaternion(1, 0, 0, 0)
		)
	)

end

function Update()

	local direction = 3;
	
	if (self.id == 2) then
		direction = - direction
	end

	if (Input.keyboard:IsPressed(Keyboard.D)) then
		self.rigidbody.velocity = vec3(direction, 0, 0)
	end
	
	if (Input.keyboard:IsPressed(Keyboard.A)) then
		self.rigidbody.velocity = vec3(-direction, 0, 0)
	end
	
	if (Input.keyboard:IsPressed(Keyboard.S)) then
		self.rigidbody.velocity = vec3(0)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.G)) then
		Physics.gravity = vec3(0, -10, 0)
	end
	
	if (Input.keyboard:IsPressed(Keyboard.F)) then
		Windows.MessageBox("Position", tostring(self.rigidbody.position))
	end

end

function Finalize()

end