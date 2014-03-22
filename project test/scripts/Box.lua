function Init()

	--Windows.MessageBox("Sup", "Sup")
	local box = Physics.Box(vec3(1, 1, 1))
	-- need some kind of shape resource manager
	
	local yo = { ["a"] = "0", ["b"] = "1", ["c"] = "2" }
	yo.sup = "yo"
	Windows.MessageBox("Test", test(yo))
	
	local boxLoader = Script:LoadScriptResourceLoader("BoxLoader")
	
	--local mesh = ResourceTypeManager:CreateResource("Mesh", "", boxLoader, { })
	
	--ResourceTypeManager:CreateResource("DX11Shader", nil, nil)
	
	--[[
	
	-- Manual resource creation
	
	local params = { ["type"] = "box" }
	local box = Physics.ShapeManager.CreateResource("BOX_COLLISION_SHAPE", ScriptResourceLoader("BoxLoader"), params)
	
	box:Load()
		
	
	--]]
	
	local position = self.transform.position
	local orientation = self.transform.orientation
	
	local mass = 1.5
	
	--Windows.MessageBox("Box", "Position: " .. tostring(position) .. "\nOrientation: " .. tostring(orientation))
	
	--self:AddComponent(RigidBodyComponent(box, mass, position, orientation))

end

function Update()

end

function Finalize()

end