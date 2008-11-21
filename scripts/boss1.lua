function move(this)
	camera = getCamera()
    mx, my, mz = getPosition(this)
    vx, vy, vz = getVelocity(this)
	cx, cy, cz = getPosition(camera)

	if cx + 40 < mx then
		return
	end

	setVelocity(this, -0.25, 0.0, 0.0)

	return
end

