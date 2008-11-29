function on_load(this)
    setVelocity(this, 0.0, 5.0, 0.0)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    mx, my, mz = getPosition(this)
    vx, vy, vz = getVelocity(this)
    cx, cy, cz = getPosition(camera)
    min_y = cy - 5.0
    max_y = cy + 5.0

    if my < min_y and vy < 0.0 then
        setVelocity(this, vx, -vy, vz)
    elseif my > 25.0 and vy > 0.0 then
        setVelocity(this, vx, -vy, vz)
    end

    return
end

function on_unload(this)
    return
end
