
Windows.AllocConsole()

local ballScript = Script:LoadBehaviour("scripts/Ball.lua")

local hObj  = GameObjects:Create()

print(hObj.sup)

hObj:AddComponent(ScriptComponent(ballScript))

print(hObj:sup())

os.execute("pause")

Game:Quit()

local ball = GameObjects:CreateTyped("Ball")

print(hObj:sup())