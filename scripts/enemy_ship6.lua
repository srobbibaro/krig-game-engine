dofile('./scripts/base_enemy.lua')

function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.19625, -2.7475, 0.0)
    setTypeId(this, 1)   
    return
end

function on_update(this, elapsedTime)
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    handle_collision(this, temp)
    return
end



