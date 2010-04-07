dofile('./scripts/base_object.lua')
dofile('./scripts/base_enemy.lua')

nextShot = 0.5
score = 100

function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, -1.57, 0.0)  
    setTypeId(this, 1)   
    return
end

function on_update(this, elapsedTime)
    plr = getPlayer()
    this_pos = getPosition(this)
    plr_pos = getPosition(plr)
    in_view = getInView(this)

    if nextShot > 0.0 then nextShot = nextShot - elapsedTime end

    if plr_pos[2] < (this_pos[2] + .5) and 
       plr_pos[2] > (this_pos[2] - .5) and 
       nextShot <= 0.0 and
       in_view == 1 then
        radius = getBoundingSphereRadius(this)
        setShot(this, "./scripts/enemy_shot.lua", radius) 
        nextShot = .85
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
