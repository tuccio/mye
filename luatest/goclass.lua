
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

b = Ball.__init()

b:MoveTo(vec3(1, 1, -1))
b:SetColor(2)

GameObject.RegisterClass("Ball", Ball)

local hBall = GameObject.new(Ball)
local hObj = GameObject.new()

print(hBall.position)
print(hBall.velocity)

os.execute("pause")