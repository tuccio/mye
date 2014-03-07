Windows.AllocConsole()

function printTransform(t)
	print(t.translation)
	print(t.rotation)
	print(t.scale)
end

hObj = GameObjects:Create("obj1")

t = transform()
printTransform(t)

hObj.transform = t
printTransform(hObj.transform)

os.execute("pause")

Game:Quit()