dofile('./scripts/base_object.lua')
dofile('./scripts/base_enemy.lua')

state = 0
nextShot = 0.5

function on_load(this)
    setModel(this, "Boss.mdl")
    setScale(this, 4.0, 4.0, 4.0)
    setRotation(this, 0.0, -1.57, 0.0)

    setVelocity(this, -5.0, 0.0, 0.0)
    life = 40
    setTypeId(this, 3)
    disableCollisionDetection(this)
    return
end

function on_update(this, elapsedTime)
    camera = getCamera()
    cam_pos = getPosition(camera)
    this_pos = getPosition(this)

    if state == 0 then
        if this_pos[1] <= (cam_pos[1] + 11.0) then
            state = 1
            setVelocity(this, 0.0, -10.0, 0.0)
            enableCollisionDetection(this)
        end
    elseif state == 1 then
        if this_pos[2] < (cam_pos[2] - 5.0) then
            state = 2
            setVelocity(this, 0.0, 10.0, 0.0)
        end
    elseif state == 2 then
        if this_pos[2] > (cam_pos[2] + 5.0) then
            state = 1
            setVelocity(this, 0.0, -10.0, 0.0)
        end
    end

    plr = getPlayer()
    plr_pos = getPosition(plr)
    in_view = getInView(this)

    if nextShot > 0.0 then nextShot = nextShot - elapsedTime end

    if plr_pos[2] < (this_pos[2] + .5) and 
       plr_pos[2] > (this_pos[2] - .5) and 
       nextShot <= 0.0 and
       in_view == 1 then
        obj = setShot(this, "./scripts/enemy_shot.lua") 
        nextShot = .85
        setScale(obj, 4.0, 4.0, 4.0)
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
