function Init()

	Windows.AllocConsole()
	Game:GetMainWindow():Focus()
	
end

function Update()

	if (not Input.keyboard:IsPressed(Keyboard.F1) and self.consoleMode) then
	
		Windows.FocusConsole()
		local code = io.read()
		
		if (code == nil or code == '') then
			self.consoleMode = false
			Game:GetMainWindow():Focus()
		else
			local f = load(code)
			f()
		end
		
	elseif (Input.keyboard:IsPressed(Keyboard.F1)) then
	
		self.consoleMode = true
		
	end

end