function Init()

	Physics.gravity = vec3(0, 0, 0)
	
	Halt()
	self.id = tonumber(self.name:sub(4))
	
	local info = { }
	
	Halt()
	if (self.id == 1) then
		info["position"] = vec3(-4, -1.17, 8.73)
		info["mass"]     = 1
	else
		info["position"] = vec3(4, -1.17, 8.73)
		info["mass"]     = 0.25
	end
	
	local box = ResourceTypeManager:CreateResource(
		"BulletCollisionShape",
		"BOX_SHAPE",
		nil,
		{ ["type"] = "box", ["halfextents"] = "1 1 1" })
		
	--self:AddComponent(RigidBodyComponent(box, info["mass"]))
	
	self.rigidbody.shape       = box
	self.rigidbody.mass        = info["mass"]
	self.rigidbody.position    = info["position"]
	self.rigidbody.orientation = quaternion(vec3(0, 1, 0), 45)
	
	self.text2d.position = vec2i(8, 1024 + (self.id - 1) * 48)
	self.text2d.color    = vec4(0, 1, 0, 1)

end

function Update()

	--local text = "Box " .. self.id .. " velocity: " .. tostring(self.rigidbody.velocity) .. " position: " .. tostring(self.rigidbody.position)
	
	local text = nil
	
	if (self.rigidbody == nil) then
		text = "Box " .. self.id .. " No rigid body attached"
	else
		text = "Box " .. self.id .. " velocity: " .. tostring(self.rigidbody.velocity) .. " position: " .. tostring(self.rigidbody.position) .. " mass: " .. tostring(self.rigidbody.mass)
	end
	
	self.text2d.text = text

end

function Finalize()

end