local v = vec3(1, 1, 1)

local hObj = GameObjects:Create()
local hObj2 = GameObjects:Create("object2")

local hFind = GameObjects:Find("object2")

print(hFind:GetName())

hFind:AddComponent("x", IntComponent())
local tmp = hFind:GetComponent("x")
tmp.value = 1337

print(hObj2:GetComponent("x").value)

print(hFind.x)
hFind.x = 7331
print(hFind.x)

os.execute("pause")