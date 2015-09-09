local DeveloperConsole = { }

function DeveloperConsole:Init()

	Windows.CreateConsole()
	Game:GetMainWindow():Focus()
	
end

function DeveloperConsole:Update()

	if (not Input.keyboard:IsPressed(KeyboardVK.F1) and self.consoleMode) then
	
		Windows.FocusConsole()
		local code = io.read()
		
		if (code == nil or code == '') then
			self.consoleMode = false
			Game:GetMainWindow():Focus()
		else
			local f = load(code)
			
			if not pcall(f) then
				print('Error executing the line: ' .. code)
			end
			
		end
		
	elseif (Input.keyboard:IsPressed(KeyboardVK.F1)) then
	
		self.consoleMode = true
		
	end

end

return DeveloperConsole