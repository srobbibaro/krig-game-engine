function on_load(this)
    setVelocity(this, vx, -10.0, vz)
    return
end

function on_update(this)
    camera = getCamera()
    mx, my, mz = getPosition(this)
    vx, vy, vz = getVelocity(this)
    cx, cy, cz = getPosition(camera)

    if my < 5.0 and vy < 0.0 then
        setVelocity(this, vx, (vy * -1), vz)
    elseif my > 25.0 and vy > 0.0 then
        setVelocity(this, vx, (vy * -1), vz)
    end

    return
end

function on_unload(this)
    return
end
