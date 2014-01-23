type = "Ball"

red = ColorRGBA(1, 0, 0, 1)
green = ColorRGBA(0, 1, 0, 1)
blue = ColorRGBA(0, 0, 1, 1)
black = ColorRGBA(0, 0, 0, 1)

Graphics.Reinterpret("DX11")

function Update(hObj, dt)

	local color = black

	if (Input:Keyboard():IsPressed(Keyboard.A)) then
		color = color + red
	end

	if (Input:Keyboard():IsPressed(Keyboard.D)) then
		color = color + green
	end

	if (Input:Keyboard():IsPressed(Keyboard.S)) then
		color = color + blue
	end

	Graphics:SetClearColor(color)

end