function on_load(this)
    setModel(this, "GreenShot.mdl")
    setTypeId(this, 20)
    enableAlwaysLit(this)

    playSound(this, "laser.wav")
    setScale(this, 4.0, 4.0, 4.0)

    return
end

function on_update(this, elapsedTime)
    in_view = getInView(this)

    if in_view == 0 then
        removeObject(this)   
    end 
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    typeId = getTypeId(temp)

    if typeId ~= 1 and typeId ~= 3 and typeId ~= 2 then
        removeObject(this)
    end
    return 
end
