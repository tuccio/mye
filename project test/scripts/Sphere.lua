function Init()

	local sphere = ResourceTypeManager:CreateResource(
		"BulletCollisionShape",
		"SPHERE_SHAPE",
		nil,
		{ ["type"] = "sphere", ["radius"] = "1" })
		
	self:AddComponent(RigidBodyComponent(sphere, 1))
	
	self.rigidbody.position = vec3(0, -1.17, 4.73)
	--self.rigidbody.scaling  = vec3(3, 3, 3)
	

end

function Update()

	if (Input.keyboard:IsPressed(Keyboard.M)) then
		self.render.matrix = Math.ScaleMatrix4(vec3(1.1)) * self.render.matrix
	end
	
	if (Input.keyboard:IsPressed(Keyboard.N)) then
		self.render.matrix = Math.ScaleMatrix4(vec3(0.9)) * self.render.matrix
	end

end