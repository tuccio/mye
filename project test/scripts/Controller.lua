function Init()
	
	self.box1 = GameObjects:Find("box1")
	self.box2 = GameObjects:Find("box2")
	
	local verdana = ResourceTypeManager:CreateResource(
		"Font",
		"verdana_regular_17",
		nil,
		nil)
		
	self:AddComponent(Text2DComponent())
	
	self.text2d.font     = verdana
	self.text2d.position = vec2i(8, 8)
	self.text2d.color    = vec4(1, 1, 0, 1)
	
end

function Update()
	
	local pressedButtons = { }
	
	if (Input.keyboard:IsPressed(Keyboard.D)) then
	
		pressedButtons[#pressedButtons + 1] = 'D'
		
		box1.rigidbody.velocity = vec3(0)
		box1.rigidbody.acceleration = 1
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.A)) then
	
		pressedButtons[#pressedButtons + 1] = 'A'
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.S)) then
	
		pressedButtons[#pressedButtons + 1] = 'S'
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Q)) then
	
		pressedButtons[#pressedButtons + 1] = 'Q'
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.E)) then
	
		pressedButtons[#pressedButtons + 1] = 'E'
		
	end
	
	local pressedButtonsString = ""
	
	for i, k in pairs(pressedButtons) do
	
		if (i == 1) then
			pressedButtonsString = k
		else
			pressedButtonsString = pressedButtonsString .. ", " .. k
		end
	
	end
	
	self.text2d.text = "Pressed buttons: " .. pressedButtonsString

end