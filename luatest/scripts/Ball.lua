type = "Ball"

function Init(hObj)

	hObj:AddComponent("position", Vec3Component())
	hObj:AddComponent("velocity", Vec3Component())
	
	hObj.position = vec3(0, 0, 0)
	hObj.velocity = vec3(1, 1, 1)
	
end

function Update(hObj, dt)

	hObj.position = hObj.position + hObj.velocity

end

function sup()

	return "sup bro"

end