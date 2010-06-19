dofile('./scripts/base_enemy.lua')
dofile('./scripts/base_shooting_object.lua')

function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, -1.57, 0.0)  
    setTypeId(this, 1)   

    setupShots(this, "./scripts/enemy_shot.lua", 0.5)
    
    score = 100
    return
end

function on_update(this, elapsedTime)
    update_shots(elapsedTime)

    plr = getPlayer()
    this_pos = getPosition(this)
    plr_pos = getPosition(plr)
    in_view = getInView(this)


    if plr_pos[1] < (this_pos[1] - 20.0) and 
       plr_pos[1] > (this_pos[1] - 30.0) and 
       in_view == 1 then
        direction = { 0.0, this_pos[1] - plr_pos[1], 0.0, 0.0 }
        direction = vector_normalize(direction)
        attemptShotWithDirection(this, direction, getBoundingSphereRadius(this))
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
