type = "Ball"

function Init(hObj)

	hObj:AddComponent(Vec3Var("position"))
	hObj:AddComponent(Vec3Var("velocity"))
	
	hObj.position = vec3(0, 0, 0)
	hObj.velocity = vec3(1, 1, 1)
	
end

function Update(hObj, dt)

	hObj.position = hObj.position + hObj.velocity

end

function sup()

	return "sup bro"

end