
Ball = { }
Ball.__index = Ball

function Ball.__init()

	local ball =
	{
		position = vec3(0, 0, 0),
		velocity = vec3(0, 0, 0),
		color = 0
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

local hObj = GameObject:new()
local hBall = GameObject:new("Ball")

hBall:MoveTo(vec3(1, 1, 1))

print(hBall.position)
print(hBall.velocity)

os.execute("pause")