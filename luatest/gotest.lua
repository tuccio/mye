Windows.AllocConsole()

GameObjects.lol = 1337
print(GameObjects.lol)

GameObjects.Create = nil
print(GameObjects.Create)

local hObj = GameObjects:Create()
local hObj_copy = hObj
local hObj2 = GameObjects:Create()

print(hObj:Exists())
print(hObj_copy:Exists())
print(hObj2:Exists())

hObj:Destroy()

print(hObj:Exists())
print(hObj_copy:Exists())
print(hObj2:Exists())

local hObj3 = GameObjects:Create()
print(hObj:Exists())

hObj2.x = 3
print(hObj2.x)

hObj2:AddComponent(IntVar("x"))
hObj2.x = 3
print(hObj2.x)

hObj2:AddComponent(StringVar("sup"))
hObj2.sup = "sup brah"
print(hObj2.sup)

hObj2:AddComponent(Vec3Var("position"))
hObj2.position = vec3(0.42141, 0.12219, 0.2131)
print(hObj2.position)

q = quaternion(1, 2.12, 3.7218, 4.12313)
q.w = -1
print(q)

t = transform()
t.translation = hObj2.position
t.rotation = q
t.scale = vec3(1, 1, 1)

print(t.translation)
print(t.rotation)
print(t.scale)

hObj2:AddComponent(TransformVar("t"))

hObj2.t = t

print(hObj2.t.translation)
print(hObj2.t.rotation)
print(hObj2.t.scale)

os.execute("pause")

Game:Quit()