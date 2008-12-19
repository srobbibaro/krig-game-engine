state = 0

function on_load(this)
    setModel(this, "Boss.mdl")
    setScale(this, 4.0, 4.0, 4.0)
    setRotation(this, 0.0, -1.57, 0.0)

    setVelocity(this, -5.0, 0.0, 0.0)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    cam_pos = getPosition(camera)
    this_pos = getPosition(this)

    if state == 0 then
        if this_pos[1] <= (cam_pos[1] + 11.0) then
            state = 1
            setVelocity(this, 0.0, -10.0, 0.0)
        end
    elseif state == 1 then
        if this_pos[2] < (cam_pos[2] - 5.0) then
            state = 2
            setVelocity(this, 0.0, 10.0, 0.0)
        end
    elseif state == 2 then
        if this_pos[2] > (cam_pos[2] + 5.0) then
            state = 1
            setVelocity(this, 0.0, -10.0, 0.0)
        end
    end

    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    return 
end
