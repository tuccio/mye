--Windows.MessageBox("Main", "Main running!")

local verdana = ResourceTypeManager:CreateResource(
	"Font",
	"verdana_regular_17",
	nil,
	nil)
	
verdana:Load()

--local text = GameObjects:Create()

--text.AddComponent(Text2DComponent(vec2i(0, 0), verdana, "Test text"))

--Windows.MessageBox("Test", sup(verdana))
--Windows.MessageBox("Test", sup(vec2i(0, 0), verdana, "Test text"))
--Text2DComponent(vec2i(0, 0), verdana, "Test text")

--[[

text:AddComponent(Text2DComponent())

text.text2d.text = "This is a simple test (or text?)"
text.text2d.position = vec2i(8, 8)
text.text2d.font = verdana
text.text2d.pointsize = vec2(1 / 800, 1 / 600)
text.text2d.color = vec4(1, 0, 0, 1)

Scene:AddGameObject(text)

]]--

local controller = GameObjects:CreateEntity("Controller")
Scene:AddGameObject(controller)