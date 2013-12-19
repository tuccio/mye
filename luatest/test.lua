--local factory = GameObjectFactory.CreateFactory("a")

--local hObj = factory:CreateObject()

local hObj = GameObject.new()

hObj.n = 10
hObj.s = "sweg"
hObj.v = { 0, 1, 0 }

--[[
local hObj = GameObject.new()

print(hObj:Exists())

hObj:Destroy()

print(hObj:Exists())

hObj:hello()

]]--

-- hObj.transform


--[[
function printVector(v)
	print(v.x, v.y, v.z)
end

local v = vec3()

--[[
local u = vec3(3, 4, 5)
local t = vec3(1)
local z = u + t
]]--

--[[
printVector(v)
v.x = 1
printVector(v)

local v2 = vec3(v)

printVector(v2)

local hFac = GetFactory()

local hObj = hFac:CreateObject()

local obj = hFac:GetObject(hObj);

--[[

printVector(v)
printVector(u)
printVector(t)
printVector(z)

--print(v)
--print(u)
--print(t)

--print(v:x())

--local x = u:dot(v)

--]]

os.execute("pause")