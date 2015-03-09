--[[
local hObj = GameObjects:Create("le_objectl0l")

hObj.transform.position.x = 10

print(hObj.transform.position)
print(hObj.transform.orientation)
print(hObj.transform.scale)
]]--

local hObj = GameObjects:CreateEntity('Test', 'test_object')
local hCam = GameObjects:CreateEntity('Camera', 'camera')

--hObj:Debug()

print ( hCam )

Scene.camera = hCam.camera

Scene:AddGameObject(hObj)
Scene:AddGameObject(hCam)

--hObj:Destroy()