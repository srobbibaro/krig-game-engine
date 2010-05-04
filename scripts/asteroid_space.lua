dofile('./scripts/base_enemy.lua')

score = 500

function on_load(this) 
    setModel(this, "Asteroid.mdl")
    setScale(this, 5.0, 5.0, 5.0)
    setRotation(this, 0.0, 0.0, 0.0)
    life = 3

    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)

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
