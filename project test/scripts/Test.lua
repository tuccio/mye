local Test = { }

function Test:Init()

	print('Init: ' .. tostring(self))
	
	self.t = (vec2(1, 2) == vec2(1, 2))
	self.f = (vec3(1, 2, 3) == vec3(0, 1, 2))
	
	self.i = 1337
	self.o = self
	self.s = self.name
	
	self.v2.x = -1
	self.v3.y = -2
	
	self.v4 = vec4( - self.v3, 666 )
	
end

function Test:Finalize()

	print('Finalize: ' .. tostring(self))
	
end
	
function Test:Update()

end

function Test:Debug()

	local t = { 'f', 't', 'i', 'r', 's', 'o', 'v2', 'v3', 'v4' }
	
	for k, v in ipairs(t) do
	
		print(v .. ': ' .. tostring(self[v]))
	
	end
	
	print ( 'Transform: ' .. tostring(self.transform.position) .. '; ' .. tostring(self.transform.orientation) .. '; ' .. tostring(self.transform.scale) )

end

return Test