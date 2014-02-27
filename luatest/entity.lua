Windows.AllocConsole()

local hObj = GameObjects:CreateEntity("Ball")

print("velocity: " .. tostring(hObj.velocity))
print("position: " .. tostring(hObj.position))

print("radius: " .. tostring(hObj.radius))

local hObj2 = GameObjects:CreateEntity("Sample")

print("name: " .. tostring(hObj2.name))
print("position: " .. tostring(hObj2.position))
print("random: " .. tostring(hObj2.random))
print("rotation: " .. tostring(hObj2.rotation))
print("visible: " .. tostring(hObj2.visible))