dofile('./scripts/base_enemy.lua')

score = 500

function on_load(this) 
    camera = getCamera()
    camera_position = getPosition(camera)

    setModel(this, "Asteroid.mdl")
    setScale(this, 2.0, 2.0, 2.0)

    setPosition(this, camera_position[1] + 20.0, math.random(30), 0.0)
    setVelocity(this, -30.0, 0.0, 0.0)

    life = 3
    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

    if this_position[1] < camera_position[1] - 20 then
        removeObject(this)
    end

    return
end

function on_unload(this) 
    return
end

function on_collision(this, temp)
    handle_collision(this, temp)

    tempId = getTypeId(temp)

    if tempId == 1 or tempId == 0 then
        removeObject(this)
    end

    return
end
