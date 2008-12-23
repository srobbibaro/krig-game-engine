duration = 0.0

function on_load(this)
    setModel(this, "blueshot.mdl")
    setTypeId(this, 2)

    playSound(this, "laser.wav")

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
    typeId = getTypeId(temp)

    if typeId ~= 0 then
        removeObject(this)
    end
    return 
end
