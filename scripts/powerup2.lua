duration = 0.0

function on_load(this)
    setModel(this, "Powerup2.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotationVelocity(this, 0.0, 2.0, 0.0);
    setTypeId(this, 6);
    return
end

function on_update(this, elapsedTime)
    duration = duration + elapsedTime

    if duration >= 2.0 then
        removeObject(this)   
    end

    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    typeId = getTypeId(temp)
    if typeId == 0 then
        playSound(this, "pause.wav")
        removeObject(this)
    end
    return
end
