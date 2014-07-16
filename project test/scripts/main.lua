--Windows.MessageBox("Main", "Main running!")

local verdana = ResourceTypeManager:CreateResource(
	"Font",
	"verdana_regular_17",
	nil,
	nil)
	
verdana:Load()

--local light = GameObjects:CreateEntity("Pointlight", "light1")
--Scene:AddGameObject(light)

--[[local controller = GameObjects:CreateEntity("Controller", "controller")

Scene:AddGameObject(controller)

local light = GameObjects:CreateEntity("MovingLight", "light1")

Scene:AddGameObject(light)
--]]

--[[
Windows.AllocConsole()

local printMatrix = function(m, padding)

	for i = 0, 3 do
	
		for j = 0, 3 do
		
		local number = tostring(m["m" .. tostring(i) .. tostring(j)])
		local spacing = "," .. string.rep(" ", padding - number:len())
		
		if (j == 3) then
			spacing = "\n"
		end
		
		io.write(number .. spacing)
		
		end
	
	end
end

local quaternion = quaternion(vec3(0, 1, 0), 45)
local rotation = Math.RotationMatrix3(quaternion)

local translation = vec3(2, 0, 0)

local m1 = Math.TranslationMatrix4(translation) * mat4(rotation)
local m2 = Math.RotationTranslationMatrix4(quaternion, translation)

local transform = transform()

transform.orientation = quaternion
transform.position = translation
transform.scale = vec3(1)

local m3 = transform.matrix

print("*** M1 ***")
printMatrix(m1, 25)

print("*** M2 ***")
printMatrix(m2, 25)

print("*** M3 ***")
printMatrix(m3, 25)

print("Test... " .. ((m1 == m2 and m2 == m3) and "OK" or "FAILED"))
]]--