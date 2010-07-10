dofile('./scripts/base_enemy.lua')

--score = 500
go_up = 1

function on_load(this) 
    setModel(this, "CannonBall.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, 0.0, 0.0)
    life = 3

--    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

    local offset = this_position[1] - 20
    if camera_position[1] >= offset then
        local y_vel = 2.0
        local z_vel = 30.0

        if this_position[2] <= 12.5 then
            y_vel = 15.0
            z_vel = 12.0
        elseif this_position[2] <= 15.0 then
            y_vel = 12.0
            z_vel = 15.0
        elseif this_position[2] <= 17.5 then
            y_vel = 10.0
            z_vel = 17.5
        elseif this_position[2] <= 19.0 then
            y_vel = 8.0
            z_vel = 20.0
        elseif this_position[2] <= 20.0 then
            y_vel = 6.0
            z_vel = 25.0
        end 

        setVelocity(this, 0.0, y_vel, z_vel )
    end 

    return
end

function on_unload(this) 
    return
end

function on_collision(this, temp)
    handle_collision(this, temp)
    return
end
