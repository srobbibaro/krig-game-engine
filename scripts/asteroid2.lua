function on_load(this)
    return
end

function on_update(this)
    camera = getCamera()
    mx, my, mz = getPosition(this)
    cx, cy, cz = getPosition(camera)

	tx = cx + 40

    if tx >= mx then
        setVelocity(this, 3.0, -5.0, 0.0)
        setRotationVelocity(this, 3.0, 3.0, 0.0)
	end 

	return
end

function on_unload(this)
    return
end
