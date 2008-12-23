dofile('./scripts/base_enemy.lua')

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

    tx = camera_position[1] + 40.0

    if tx >= this_position[1] then
        setVelocity(this, 3.0, -5.0, 0.0)
        setRotationVelocity(this, 3.0, 3.0, 0.0)
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
