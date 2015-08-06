local ShadowDebug = { }

function ShadowDebug:Init()
end

function ShadowDebug:Update()
end

function ShadowDebug:OnKeyboardKeyPress(key)

	if key == KeyboardVK.M then
		r.csmSplits = r.csmSplits + 1
	elseif key == KeyboardVK.N then
		r.csmSplits = r.csmSplits - 1
	elseif key == KeyboardVK.K then
		r.csmDebug = not r.csmDebug
	elseif key == KeyboardVK.J then
		r.pcfEnabled = not r.pcfEnabled
	end
	
end

return ShadowDebug