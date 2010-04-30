dofile('./scripts/base_object.lua')
dofile('./scripts/base_enemy.lua')
dofile('./scripts/base_shooting_object.lua')

state = 0
nextShot = 3.5
score = 2000

function on_load(this)
    setModel(this, "Boss.mdl")
    setScale(this, 4.0, 4.0, 4.0)
--    setRotation(this, 0.0, -1.57079633, 0.0)
    setRotation(this, 0.0, -1.5708, 0.0)

    setVelocity(this, -5.0, 0.0, 0.0)
    life = 40
    setTypeId(this, 3)
    disableCollisionDetection(this)

    setupShots(this, './scripts/boss_shot.lua', 0.85)
    return
end

function on_update(this, elapsedTime)
    update_shots(elapsedTime)

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

--    if plr_pos[2] < (this_pos[2] + .5) and 
--       plr_pos[2] > (this_pos[2] - .5) and 
    if in_view == 1 then
        radius = getBoundingSphereRadius(this) - 1.25
        attemptShots(this, 2, {radius, radius}, {radius, -radius})
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
