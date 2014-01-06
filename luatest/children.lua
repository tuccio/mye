Windows.AllocConsole()

function DFSGameObjects(o)

	children = o:GetChildren()

	io.write(tostring(o) .. " { ")

	for i, v in ipairs(children) do
		
		DFSGameObjects(v)

	end

	io.write(" } ")

end

o1 = GameObjects:Create("o1")

o11 = GameObjects:Create("o11")
o11:SetParent(o1)

o12 = GameObjects:Create("o12")
o12:SetParent(o1)

o121 = GameObjects:Create("o121")
o121:SetParent(o12)

o122 = GameObjects:Create("o122")
o122:SetParent(o12)

DFSGameObjects(o1)

print("\nSet o12 parent = nil")
o12:SetParent(nil)

DFSGameObjects(o1)
print()

DFSGameObjects(o12)
print()

print("o12 parent: " .. tostring(o12:GetParent()) or "nil")
print("o121 parent: " .. tostring(o121:GetParent()) or "nil")