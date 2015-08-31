local SponzaController = { }

function SponzaController:Init()
	
	Graphics.window.caption = 'Sponza test'
	
	r.shadowMapBias = 0.001
	r.pcfEnabled    = false
	r.csmSplits     = 1
		
	r.lpvEnabled    = true
	r.lpvIterations = 26
	r.lpvAABB       = AABB.FromMinMax(vec3(-20), vec3(20))
	
	--r.lpvAABB       = AABB.FromMinMax(vec3(-200), vec3(200))
	
	local hCam = GameObjects:CreateEntity('Camera', 'camera')
	
	if hCam and hCam:Exists() then
	
		hCam.transform.position    = vec3(-8.47068, 3.23661, -0.0175723)
		hCam.transform.orientation = quaternion(0.730156, 0.0297107, 0.68207, -0.0277546)
	
		hCam.speed       = 3
		--hCam.speed       = 300
	
		hCam.camera.fovy = 70
		hCam.camera.near = 0.01
		hCam.camera.far  = 50
		--hCam.camera.far  = 5000
		
		Scene.camera = hCam.camera
		
	else
		System.PopupMessage('Cannot create camera')
		Game:Quit()
	end
	
	local hShadowDebug = GameObjects:CreateEntity('ShadowDebug', 'shadowDebug')
	
end

function SponzaController:Update()

end

function SponzaController:OnKeyboardKeyHold(key, t)

	local angle = 25 * Time.delta
	local v     = vec3(1, 0, 0.1):Normalize()

	if key == KeyboardVK.R then
	
		local hLight = GameObjects:Find('sun')
		
		local q = quaternion(v, angle)
		hLight.light.direction = q:Rotate(hLight.light.direction):Normalize()
		
	end
	
	if key == KeyboardVK.T then
	
		local hLight = GameObjects:Find('sun')
		
		local q = quaternion(v, - angle)
		hLight.light.direction = q:Rotate(hLight.light.direction):Normalize()
		
	end

end

function SponzaController:OnKeyboardKeyPress(key)

	if key == KeyboardVK.F then
	
		local hFrustumRenderer = GameObjects:Find('frustumRenderer')
		
		if not hFrustumRenderer:Exists() then
			hFrustumRenderer = GameObjects:CreateEntity('FrustumRenderer', 'frustumRenderer')
			Scene:AddGameObject(hFrustumRenderer)
		end
		
		hFrustumRenderer.camera:SetCamera(Scene.camera)
		
	elseif key == KeyboardVK.G then
		
		r.lpvEnabled = not r.lpvEnabled
		
	end

end

return SponzaController