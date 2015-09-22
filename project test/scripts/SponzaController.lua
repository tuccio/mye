local SponzaController = { }

function SponzaController:Init()
	
	Graphics.window.caption = 'Sponza'
	Graphics.vsync          = true
	
	self:SetupCamera()
	self:SetupShadowMapping()
	self:SetupLPV()
	self:SetupPostProcessing()
	
end

function SponzaController:Update()

end

function SponzaController:SetupShadowMapping()

	r.shadowMapResolution       = 1024
	
	r.vsmEnabled                = true
	r.vsmExponential            = true
	r.vsmMinBleeding            = 0.25
	r.vsmMinVariance            = 0.0001
	r.vsmBlur                   = 21
	
	r.esmPositiveExponent       = 30
	r.esmNegativeExponent       = -1
	
	r.pcfEnabled                = false
	r.shadowMapBias             = 0
	r.shadowMapNormalOffsetBias = 0
	r.csmSplits                 = 1
	
end

function SponzaController:SetupCamera()

	local hCam = GameObjects:CreateEntity('Camera', 'camera')
	
	if hCam and hCam:Exists() then
	
		hCam.transform.position    = vec3(-8.67944, 3.60177, 0.410704)
		hCam.transform.orientation = quaternion(0.693654, 0.0710503, 0.713065, -0.0730392)
	
		hCam.speed       = 3
	
		hCam.camera.fovy = 70
		hCam.camera.near = 0.01
		hCam.camera.far  = 35
		
		Scene.camera = hCam.camera
		
	else
		System.PopupMessage('Cannot create camera')
		Game:Quit()
	end

end

function SponzaController:SetupLPV()

	r.lpvEnabled    = true
	r.lpvIterations = 24
	r.lpvResolution = 32
	r.lpvAABB       = AABB.FromMinMax(vec3(-40), vec3(40))
		
	r.lpvFluxInjectionBias     = 0.5
	r.lpvGeometryInjectionBias = 0
	
end

function SponzaController:SetupPostProcessing()

	r.ppTonemapMiddleGrey = 0.18
	r.ppTonemapLogAverage = 0.25
	r.ppTonemapWhite      = 2
	
end

function SponzaController:OnKeyboardKeyHold(key, t)

	local angle = 10 * Time.delta
	local v     = vec3(1, 0.3, -1):Normalize()

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
		
		local enabled = not r.lpvEnabled
		r.lpvEnabled  = enabled
		
		print ('LPV: ' .. (enabled and 'ON' or 'OFF'))
		
	elseif key == KeyboardVK.P then
	
		local newIt     = math.min(r.lpvIterations + 1, 32)
		r.lpvIterations = newIt
		
		print('Iterations: ' .. newIt)
	
	elseif key == KeyboardVK.O then
	
		local newIt     = math.max(r.lpvIterations - 1, 0)
		r.lpvIterations = newIt
		
		print('Iterations: ' .. newIt)
		
	elseif key == KeyboardVK.H then
	
		local hLPVAnim = GameObjects:Find('lpvAnim')
		
		if not hLPVAnim:Exists() then
			hLPVAnim = GameObjects:CreateEntity('LPVAnimationController', 'lpvAnim')
			hLPVAnim.maxIterations = 24
		else
			hLPVAnim:Restart()
		end
		
	elseif key == KeyboardVK.K then

		r.lpvAttenuation = 10000
		r.lpvIterations  = 7
		
	end

end

return SponzaController