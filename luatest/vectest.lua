local v1 = vec3(0, 0, 0)
print("v1 : " .. tostring(v1));
local v2 = vec3(0, 0, 0)
print("v2 : " .. tostring(v2));

print("v1 == v2 : " .. tostring(v1 == v2))
print("v1 ~= v2 : " .. tostring(v1 ~= v2))

local v3 = vec3(0, 0, 0)

v3.x = -3
v3.y = -2
v3.z = -1

print("v3 : [x = " .. v3.x .. "; y = " .. v3.y .. "; z = " .. v3.z .. "]")

print("v3 : " .. tostring(v3))

print("v1 == v3 : " .. tostring(v1 == v3))
print("v1 ~= v3 : " .. tostring(v1 ~= v3))

local v4 = vec3(1, 2, 3)
print("v4 : " .. tostring(v4));

print("v3 + v4 : " .. tostring(v3 + v4))
print("v3 - v4 : " .. tostring(v3 - v4))
print("v3 * v4 : " .. tostring(v3 * v4))
print("v3 / v4 : " .. tostring(v3 / v4))

local v5 = vec3(1, 1, 1)
print("v5 : " .. tostring(v5))

print("v5 + 1 : " .. tostring(v5 + 1))
print("v5 - 1 : " .. tostring(v5 - 1))
print("v5 * 2 : " .. tostring(v5 * 2))
print("v5 / 2 : " .. tostring(v5 / 2))

os.execute("pause")