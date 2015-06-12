local CornellController = { }

function CornellController:Init()
	
	local hCam = GameObjects:CreateEntity('camera', 'Camera')
	
	hCam.transform.position = vec3(0, 1, 3)
	hCam.transform.orientation = quaternion(0, 0, 1, 0)
	
	print (hCam.transform.position)
	print (hCam.transform.orientation)
	
	if hCam and hCam:Exists() then
		hCam.camera.far = 25
		Scene.camera = hCam.camera
		
	else
		System.PopupMessage('Cannot create camera')
		Game:Quit()
	end
	
end

function CornellController:Update()

	--Graphics.window.caption = "Cornell box [FPS: " .. math.floor(Graphics.fps) .. "]"

	--print(self.transform.position)
	
	local hLight = GameObjects:Find('light1')
	
	self.text2d.text = 'Light direction: ' .. tostring(hLight.light.direction)

end

function CornellController:OnKeyboardKeyHold(key, t)

	local angle = 25 * Time.delta
	local v     = vec3(0, 0, -1)

	if key == KeyboardVK.R then
	
		local hLight = GameObjects:Find('light1')
		
		local q = quaternion(v, angle)
		hLight.light.direction = q:Rotate(hLight.light.direction):Normalize()
		
	end
	
	if key == KeyboardVK.T then
	
		local hLight = GameObjects:Find('light1')
		
		local q = quaternion(v, - angle)
		hLight.light.direction = q:Rotate(hLight.light.direction):Normalize()
		
	end

end

return CornellController