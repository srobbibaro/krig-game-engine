function move()
      mx, my, mz = getPosition()
      vx, vy, vz = getVelocity()
	cx, cy, cz = getCameraPosition()

	if cx + 10 < mx then
		return
	end

	if my < 5.0 and vy < 0.0 then
		setVelocity(vx, (vy * -1), vz)
	elseif my > 25.0 and vy > 0.0 then
		setVelocity(vx, (vy * -1), vz)
	elseif vy == 0.0 then
		setVelocity(vx, -10.0, vz)
	end

	return
end

