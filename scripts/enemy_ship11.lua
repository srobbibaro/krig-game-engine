dofile('./scripts/base_enemy.lua')
dofile('./scripts/base_shooting_object.lua')

state = 1
orig_height = 0.0

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

    if state == 1 then
        if plr_pos[1] > this_pos[1] - 25.0 then
            setVelocity(this, 0.0, 5.0, 0.0) 
            orig_height = this_pos[2]
            state = 2
        end
    elseif state == 2 then
        if this_pos[2] >= orig_height + 3.0 then 
            setVelocity(this, 0.0, 0.0, 0.0) 
            setSpeed(this, 40.0, 0.0, 0.0)
            state = 3
        end    
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
