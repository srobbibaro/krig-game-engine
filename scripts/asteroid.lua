dofile('./scripts/base_enemy.lua')

score = 500

function on_load(this) 
    setModel(this, "Asteroid.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, 0.0, 0.0)
    life = 3

    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

    -- Set seed for random positions and velocities.
    math.randomseed( this_position[1] )

    -- Stupid line for Windows... 
    -- Throws out the first (faulty) random value.
    math.random(40);

    tx = camera_position[1] + math.random(0, 40);

    if tx >= this_position[1] then
        setVelocity(this, math.random(0,3), -math.random(5, 7), 0.0)
        setRotationVelocity(this, 
            math.random(0, 3), math.random(0, 3), math.random(0, 3))
    end 

    if this_position[2] < camera_position[2] - 20 then
        removeObject(this)
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
