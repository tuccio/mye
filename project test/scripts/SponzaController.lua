local SponzaController = { }

function SponzaController:Init()
	
	Graphics.window.caption = 'Sponza test'
	
	r.shadowMapBias             = 0.001
	r.shadowMapNormalOffsetBias = 0
	r.pcfEnabled                = false
	r.pcfKernel                 = 1
	r.csmSplits                 = 1
		
	r.lpvEnabled    = false
	r.lpvIterations = 16
	r.lpvAABB       = AABB.FromMinMax(vec3(-25), vec3(25))
	
	--r.lpvAABB       = AABB.FromMinMax(vec3(-1400), vec3(1400))
	
	local hCam = GameObjects:CreateEntity('Camera', 'camera')
	
	if hCam and hCam:Exists() then
	
		hCam.transform.position    = vec3(-8.47068, 4.23661, -0.0175723)
		--hCam.transform.position    = vec3(-87, 240, -26)
		hCam.transform.orientation = quaternion(0.730156, 0.0297107, 0.68207, -0.0277546)
	
		hCam.speed       = 3
		--hCam.speed       = 150
	
		hCam.camera.fovy = 70
		hCam.camera.near = 0.01
		hCam.camera.far  = 35
		--hCam.camera.far  = 2500
		
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
	local v     = vec3(0.25, 1, -0.4):Normalize()

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