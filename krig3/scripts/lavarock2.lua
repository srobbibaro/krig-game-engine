dofile('./scripts/base_enemy.lua')

falling = 0

function on_load(this) 
    setModel(this, "redrock.mdl")
    setScale(this, 4.0, 4.0, 4.0)
    setRotation(this, 0.0, 0.0, 0.0)
    life = 3

    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

    -- Don't start moving until we're close to it.
    if camera_position[1] < this_position[1]-30.0 or 
       camera_position[1] > this_position[1]+30.0 then
        return
    end

    -- Set seed for random positions and velocities.
    math.randomseed( this_position[1] )

    -- Stupid line for Windows... Throws out the first (faulty) random value.
    math.random(40);

    setRotationVelocity(this, 
        math.random(0,3), math.random(0,3), math.random(0,3))

    if falling == 1 then
        setVelocity(this, 0.0, -20.0, 0.0)
        return
    end

    angle_height = 40  -- Change upward direction when we get here.
    fall_height  = 90  -- Move the rock over to the player when we get here

    x_vel = 0.0
    if this_position[2] >= angle_height then
        x_vel = -5.0 
    end
    setVelocity(this, x_vel, 15.0, 0.0)

    if this_position[2] >= fall_height then
        falling = 1
        math.randomseed( camera_position[1] )
        x_pos = camera_position[1] + math.random(-5,20) 
        setPosition(this, x_pos, 30.0, 7.5)
        setScale(this, 3.0, 3.0, 3.0)
    end

    return
end

function on_unload(this) 
    return
end

function on_collision(this, temp)
    return
end
