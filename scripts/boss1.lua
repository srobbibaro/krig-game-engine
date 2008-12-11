function on_load(this)
    setModel(this, "Boss.mdl")
    setScale(this, 4.0, 4.0, 4.0)
    setRotation(this, 0.0, -1.57, 0.0)

    setVelocity(this, 0.0, 5.0, 0.0)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    this_velocity = getVelocity(this)
    camera_position = getPosition(camera)
    min_y = camera_position[2] - 5.0
    max_y = camera_position[2] + 5.0

    if this_position[2] < min_y and this_position[2] < 0.0 then
        setVelocity(this, this_velocity[1], -this_velocity[2], this_velocity[3])
    elseif this_position[2] > 25.0 and this_position[2] > 0.0 then
        setVelocity(this, this_velocity[1], -this_velocity[2], this_velocity[3])
    end

    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    return 
end

