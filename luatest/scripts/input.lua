Windows.AllocConsole()

local inputScript = Script:LoadBehaviour("scripts/InputListener.lua")

local hObj  = GameObjects:Create()

hObj:AddComponent(ScriptComponent(inputScript))
