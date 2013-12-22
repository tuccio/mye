print("Set new field in GameObject")
GameObject.lol = 1337
print(GameObject.lol)

GameObject.new = nil

local hObj = GameObject.new()
local hObj_copy = hObj
local hObj2 = GameObject.new()


print(hObj:Exists())
print(hObj_copy:Exists())
print(hObj2:Exists())

hObj:Destroy()

print(hObj:Exists())
print(hObj_copy:Exists())
print(hObj2:Exists())


hObj.x = 3
hObj2.x = 3

print(hObj2.x)

os.execute("pause")