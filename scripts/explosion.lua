duration = 0.0

function on_load(this)
    setModel(this, "Explosion.mdl")
    setScaleRate(this, 1.0, 1.0, 1.0)
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
    return
end
