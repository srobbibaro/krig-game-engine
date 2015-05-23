dofile('./scripts/base_enemy.lua')

score = 500

function on_load(this) 
    setModel(this, "redrock.mdl")
    setScale(this, 1.0, 1.0, 1.0)
    setRotation(this, 0.0, 0.0, 0.0)

    setTypeId(this, 4)
    return
end

function on_update(this, elapsedTime)

    return
end

function on_unload(this) 
    return
end

function on_collision(this, temp)
    return
end
