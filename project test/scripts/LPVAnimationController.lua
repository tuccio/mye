local LPVAnimationController = { }

function LPVAnimationController:Init()
	r.lpvIterations = 0
end

function LPVAnimationController:Update()

	local elapsed = self.elapsed + Time.delta
	
	local t  = 2 * elapsed * self.speed
	local it = math.floor(t)
	
	if it < self.maxIterations then
		
		self.elapsed    = elapsed
		r.lpvIterations = it
		
	else
	
		self:Destroy()
		
	end

end

function LPVAnimationController:Restart()
	self.elapsed = 0
end

return LPVAnimationController