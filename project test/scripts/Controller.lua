function Init()
	
	self.box1 = GameObjects:Find("box1")
	self.box2 = GameObjects:Find("box2")
	self.sphere = GameObjects:Find("sphere")
	
	self.text2d.color    = vec4(1, 1, 0, 1)
	
end

function Update()
	
	local clampValue = 10
	local pressedButtons = { }
	
	local sensitivity = 0.1
	
	if (Input.keyboard:IsPressed(Keyboard.G)) then
		Physics.gravity = vec3(0, -2, 0)
	end
	
	local up = vec3(0, 1, 0)
	local right = vec3(1, 0, 0)
	local forward = vec3(1, 0, 1)
	
	if (Input.mouse:IsPressed(Mouse.Mouse1)) then
	
		pressedButtons[#pressedButtons + 1] = 'Mouse 1'
		
		local delta = Input.mouse.delta * sensitivity
		
		Scene.camera:Pitch(delta.y)
		Scene.camera:Yaw(delta.x)
	
	end
	
	if (Input.mouse:IsPressed(Mouse.Mouse2)) then
	
		pressedButtons[#pressedButtons + 1] = 'Mouse 2'
		
		local delta = Input.mouse.delta * sensitivity
		
		Scene.camera.position = Scene.camera.position + Scene.camera.right * delta.x - Scene.camera.up * delta.y
	
	end
	
	if (Input.mouse:IsPressed(Mouse.Mouse3)) then
	
		pressedButtons[#pressedButtons + 1] = 'Mouse 2'
		
		local delta = Input.mouse.delta * sensitivity
		
		Scene.camera.position = Scene.camera.position + Scene.camera.right * delta.x - Scene.camera.forward * delta.y
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Shift)) then
	
		pressedButtons[#pressedButtons + 1] = 'Shift'
		
		clampValue = 3
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Space)) then
	
		pressedButtons[#pressedButtons + 1] = 'Space'
		
		self.box1.rigidbody.velocity   = vec3(0)
		self.box2.rigidbody.velocity   = vec3(0)
		self.sphere.rigidbody.velocity = vec3(0)
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.E)) then
	
		pressedButtons[#pressedButtons + 1] = 'E'
		Scene.camera:Yaw(5)
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Q)) then
	
		pressedButtons[#pressedButtons + 1] = 'Q'
		Scene.camera:Yaw(-5)
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.C)) then
	
		pressedButtons[#pressedButtons + 1] = 'C'
		Scene.camera.position = Scene.camera.position + Scene.camera.right
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Z)) then
	
		pressedButtons[#pressedButtons + 1] = 'Z'
		Scene.camera.position = Scene.camera.position - Scene.camera.right
	
	end
	
	if (Input.keyboard:IsPressed(Keyboard.D)) then
	
		pressedButtons[#pressedButtons + 1] = 'D'
		
		self.box1.rigidbody.velocity = (self.box1.rigidbody.velocity + self.box1.transform.right):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.A)) then
	
		pressedButtons[#pressedButtons + 1] = 'A'
		
		self.box1.rigidbody.velocity = (self.box1.rigidbody.velocity - self.box1.transform.right):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.W)) then
	
		pressedButtons[#pressedButtons + 1] = 'W'
		
		self.box1.rigidbody.velocity = (self.box1.rigidbody.velocity + self.box1.transform.forward):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.S)) then
	
		pressedButtons[#pressedButtons + 1] = 'S'
		
		self.box1.rigidbody.velocity = (self.box1.rigidbody.velocity - self.box1.transform.forward):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.LeftArrow)) then
	
		pressedButtons[#pressedButtons + 1] = 'LeftArrow'
		
		self.box2.rigidbody.velocity = (self.box2.rigidbody.velocity - self.box1.transform.right):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.RightArrow)) then
	
		pressedButtons[#pressedButtons + 1] = 'RightArrow'
		
		self.box2.rigidbody.velocity = (self.box2.rigidbody.velocity + self.box1.transform.right):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.UpArrow)) then
	
		pressedButtons[#pressedButtons + 1] = 'UpArrow'
		
		self.box2.rigidbody.velocity = (self.box2.rigidbody.velocity + self.box1.transform.forward):Clamp(-clampValue, clampValue)
		
	end
	
	if (Input.keyboard:IsPressed(Keyboard.DownArrow)) then
	
		pressedButtons[#pressedButtons + 1] = 'DownArrow'
		
		self.box2.rigidbody.velocity = (self.box2.rigidbody.velocity - self.box1.transform.forward):Clamp(-clampValue, clampValue)
		
	end
	
	local pressedButtonsString = ""
	
	for i, k in pairs(pressedButtons) do
	
		if (i == 1) then
			pressedButtonsString = k
		else
			pressedButtonsString = pressedButtonsString .. ", " .. k
		end
	
	end
	
	self.text2d.text = "Pressed buttons: " .. pressedButtonsString .. "\nPhysical bodies count: " .. Physics:GetBodiesCount()

end