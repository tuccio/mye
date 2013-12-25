Ball = { }
Ball.__index = Ball

function Ball.__init()

	print("Constructor called")

	local ball =
	{
		position = vec3(0, 0, 0),
		velocity = vec3(0, 0, 0),
		color = 1
	}

	setmetatable(ball, Ball)

	return ball

end

function Ball:MoveTo(x)
	self.position = x
end

function Ball:SetColor(c)
	self.color = c
end

GameObject:RegisterClass("Ball", Ball)

local hObj = GameObject:new()
print("Object 1 created, name: " .. (hObj.name or "nil"))

local hBall = GameObject:new("Ball", nil)
print("Ball 1 created, name: " .. (hBall.name or "nil"))

local hBall2 = GameObject:new("Ball", "ball2")
hBall2.position = vec3(-1, -1, -1)
print("Ball 2 created, name: " .. (hBall2.name or "nil"))

local hObj2 = GameObject:new(nil, "obj2")
print("Object 2 created, name: " .. (hObj2.name or "nil"))

hBall:MoveTo(vec3(1, 1, 1))

print(hBall.position)
print(hBall.velocity)
print(hBall.color)

hBall.color = "red"
hBall.sweg = true

print(hBall.color)
print(hBall.sweg)

local ball2 = GameObject:Find("ball2")
print("Ball 2 found:")
print(ball2.position)

os.execute("pause")