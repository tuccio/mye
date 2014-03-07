
Windows.AllocConsole()

local b1 = GameObjects:CreateEntity("Ball", "b1")

print(b1.position)

local dt = 1;

print(type(dt))

b1:Update(b1, 1)

print(b1.position)