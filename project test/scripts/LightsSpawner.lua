local LightSpawner = {}

function LightSpawner:Init()

	local hLight = GameObjects:CreateEntity("RotatingLight")
	
	if (not hLight:Exists()) then
		Windows.MessageBox("Error", "Cannot create light")
		Game:Quit()
	end
	
	--self:Destroy()

end

return LightSpawner