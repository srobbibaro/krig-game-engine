function move(this)
    camera = getCamera()
    mx, my, mz = getPosition(this)
    vx, vy, vz = getVelocity(this)
	cx, cy, cz = getPosition(camera)

	if cx + 10 < mx then
		return
	end

	if my < 5.0 and vy < 0.0 then
		setVelocity(this, vx, (vy * -1), vz)
	elseif my > 25.0 and vy > 0.0 then
		setVelocity(this, vx, (vy * -1), vz)
	elseif vy == 0.0 then
		setVelocity(this, vx, -10.0, vz)
	end

	return
end

