Windows.AllocConsole()

window = Game:GetMainWindow()

window:SetCaption("Engine Test")

window:Show()

hObj = GameObjects:Create("obj1")
print(hObj:GetName())

hi = Script:LoadProcedure("hi.lua")
hi2 = hi
hi:Run()
hi = nil
hi2:Run()
hi2 = nil