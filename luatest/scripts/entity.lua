Windows.AllocConsole()

local hObj = GameObjects:CreateEntity("Ball")


print("velocity: " .. tostring(hObj.velocity))
print("position: " .. tostring(hObj.position))

print("radius: " .. tostring(hObj.radius))

for i = 1, 10 do
	hObj:Update(1)
	print(hObj.position)
end


--[[
local hObj2 = GameObjects:CreateEntity("Sample")

print("name: " .. tostring(hObj2.name))
print("position: " .. tostring(hObj2.position))
print("random: " .. tostring(hObj2.random))
print("rotation: " .. tostring(hObj2.rotation))
print("visible: " .. tostring(hObj2.visible))
--]]