duration = 0.0

function on_load(this)
    setVelocity(this, 15.0, 0.0, 0.0)
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
