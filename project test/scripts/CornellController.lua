local CornellController = { }

function CornellController:Init()
	
	local hCam = GameObjects:CreateEntity('Camera', 'camera')
	
	hCam.transform.position    = vec3(0, 1, 3)
	hCam.transform.orientation = quaternion(0, 0, 1, 0)
	
	if hCam and hCam:Exists() then
	
		hCam.camera.near = 0.001
		hCam.camera.far  = 10	
		
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

function CornellController:OnKeyboardKeyPress(key)

	if key == KeyboardVK.F then
	
		local hFrustumRenderer = GameObjects:Find('frustumRenderer')
		
		if not hFrustumRenderer:Exists() then
			hFrustumRenderer = GameObjects:CreateEntity('FrustumRenderer', 'frustumRenderer')
			Scene:AddGameObject(hFrustumRenderer)
		end
		
		hFrustumRenderer.camera:SetCamera(Scene.camera)
		
	elseif key == KeyboardVK.V then
		
		local hCam = GameObjects:Find('camera')
		
		local viewMatrix = hCam.camera:GetViewMatrix()
		local projMatrix = hCam.camera:GetProjectionMatrix()
		
		local viewProj = projMatrix * viewMatrix
		local viewProjInv = viewProj:Inverse()
		
		local octavePrintMatrix4 = function (m)
		
			local r = '['
			
			for i = 0, 3 do
			
				for j = 0, 3 do
				
					r = r .. tostring(m:Get(i, j))
					
					if j < 3 then
						r = r .. ', '
					end
					
				end
				
				if i < 3 then
					r = r .. '; '
				end
				
			end
			
			r = r .. ']'
			
			print(r)
			
		end
		
		print('view:')
		octavePrintMatrix4(viewMatrix)
		
		print('proj:')
		octavePrintMatrix4(projMatrix)
		
		print('viewProj:')
		octavePrintMatrix4(viewProj)
		
		print('viewProjInv:')
		octavePrintMatrix4(viewProjInv)
		
	end

end

return CornellController