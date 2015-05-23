shake = 0

function on_load(this)
    setModel(this, "volcano.mdl")
    setScale(this, 20.0, 20.0, 20.0)
    setRotation(this, 0.0, 1.0, 0.0)     
    disableCollisionDetection(this)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

    -- Don't start shaking until we're close to it.
    if camera_position[1] < this_position[1]-30.0 or 
       camera_position[1] > this_position[1]+30.0 then
        return
    end

    -- We're in range, shake it baby.
    if shake == 0 then
        setScale(this, 21.0, 21.0, 21.0);
        shake = 1
    else
        setScale(this, 20.0, 20.0, 20.0);
        shake = 0
    end
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    return
end
