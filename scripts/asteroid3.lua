function move(this)
    camera = getCamera()
    mx, my, mz = getPosition(this)
    cx, cy, cz = getCameraPosition(camera)

    if cx >= mx then
        setVelocity(this, 3.0, -7.0, 0.0)
        setRotationVelocity(this, 0.0, 3.0, 3.0)
	end 

	return
end

