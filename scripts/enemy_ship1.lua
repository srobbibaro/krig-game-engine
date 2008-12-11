function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, -1.57, 0.0)  
    setTypeId(this, 1)   
    return
end

function on_update(this, elapsedTime)
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    typeId = getTypeId(temp)

    if typeId == 2 then
        playSound(this, "explosion1.wav")
        this_position = getPosition(this)

        obj = addObject(this, "./scripts/explosion.lua")
        setPosition(obj, this_position[1], this_position[2], this_position[3])
        removeObject(this)
    end
    return
end
