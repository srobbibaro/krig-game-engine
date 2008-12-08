duration = 0.0

function on_load(this)
    setModel(this, "blueshot.mdl")

    player = getPlayer()
    prx, pry, prz = getRotation(player)
    pdx, pdy, pdz = getDirection(player)

    setRotation(this, prx, pry, prz)
    setRotationVelocity(this, pdx * 8.0, pdy * 8.0, pdz * 8.0)

    -- logic here needs to be fixed
    setSpeed(this, 0, 25.0)

    return
end

function on_update(this, elapsedTime)
    duration = duration + elapsedTime

    if duration > 5.0 then
        removeObject(this)   
    end
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    removeObject(this)
    return 
end
