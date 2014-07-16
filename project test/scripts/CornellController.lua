function Update()

	Graphics.window.caption = "Cornell box [FPS: " .. math.floor(Graphics.fps) .. "]"

	--print(self.transform.position)

	local cam = Scene.camera
	
	local vdt = vec3(1) * Time.delta
	local dx = vec3(0)
	
	if (Input.mouse:IsPressed(Mouse.Mouse1)) then
		
		local ray = cam:RayCast(Input.mouse.position)
		
		local intersection = Scene:Pick(ray)
		
		if (intersection.hObj ~= self.selected) then
			self.selected = intersection.hObj
			
			if (intersection.hObj:Exists()) then
				print("Selected object: " .. tostring(intersection.hObj))
			else
				print("No object selected")
			end
			
		end
		
	end	

	if (Input.keyboard:IsPressed(Keyboard.W)) then
		dx = dx + vdt * cam.forward
	end
	
	if (Input.keyboard:IsPressed(Keyboard.S)) then
		dx = dx - vdt * cam.forward
	end
	
	if (Input.keyboard:IsPressed(Keyboard.D)) then
		dx = dx + vdt * cam.right
	end
	
	if (Input.keyboard:IsPressed(Keyboard.A)) then
		dx = dx - vdt * cam.right
	end
	
	if (Input.keyboard:IsPressed(Keyboard.E)) then
		dx = dx + vdt * cam.up
	end
	
	if (Input.keyboard:IsPressed(Keyboard.Q)) then
		dx = dx - vdt * cam.up
	end
	
	if (Input.mouse:IsPressed(Mouse.Mouse2)) then
		
		local dr = vec2(0)
		local sens = 150
		
		local delta = Input.mouse.delta
		
		Scene.camera:Yaw(math.atan(delta.x * sens))
		Scene.camera:Pitch(math.atan(delta.y * sens))
		
	end
	
	Scene.camera.position = cam.position + dx	

end

function OnMouseEvent(mouseEvent)

	--if (mouseEvent.type == MouseEvent.KeyDown) then ..

end