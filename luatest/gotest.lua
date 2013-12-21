
print("Set new field in GameObject")
GameObject.hi = 1337
print(GameObject.hi)

GameObject.new = nil

local hObj = GameObject.new()
local hObj_copy = hObj

print(hObj:Exists())
print(hObj_copy:Exists())

hObj:Destroy()

print(hObj:Exists())
print(hObj_copy:Exists())

hObj.x = 3

os.execute("pause")