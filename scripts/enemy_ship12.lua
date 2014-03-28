dofile('./scripts/base_enemy.lua')
dofile('./scripts/base_shooting_object.lua')

state      = 0
interpTime = 0.0
score      = 100

function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.0, -1.57, 0.0)
    setTypeId(this, 1)   
    setupShots(this, "./scripts/enemy_shot.lua", 0.5)
    return
end

function on_update(this, elapsedTime)
    update_shots(elapsedTime)
    this_pos = getPosition(this)
    camera = getCamera()
    cam_pos = getPosition(camera)
    in_view = getInView(this)
    player = getPlayer()
    plr_pos = getPosition(player)

    if state == 0 then
        if cam_pos[1] + 10.0 > this_pos[1] then
            this_rotation = getRotation(this)
            setInterpolationVariableBeginValue(this, 0.0)
            setInterpolationVariableEndValue(this, 0.5)
            setInterpolationVariableCurrentValue(this, 0.0)
            setInterpolationRotationStartv(this, this_rotation)
            this_rotation[3] = -1.57
            setInterpolationRotationEndv(this, this_rotation)
            setInterpolationEnable(this, 1)
            setSpeed(this, 10.0)
            state = 1
            interpTime = 0.0
        end
    elseif state == 1 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)
        
        if interpTime >= 0.5 then
            this_rotation = getRotation(this)
            setInterpolationVariableBeginValue(this, 0.0)
            setInterpolationVariableEndValue(this, 1.0)
            setInterpolationVariableCurrentValue(this, 0.0)
            setInterpolationRotationStartv(this, this_rotation)
            this_rotation[3] = -3.14
            setInterpolationRotationEndv(this, this_rotation)
            setInterpolationEnable(this, 1)
            setSpeed(this, 20.0)
            state = 2
            interpTime = 0.0
        end
    elseif state == 2 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)

        if interpTime >= 1.0 then
            this_rotation = getRotation(this)
            setInterpolationVariableBeginValue(this, 0.0)
            setInterpolationVariableEndValue(this, 1.0)
            setInterpolationVariableCurrentValue(this, 0.0)
            setInterpolationRotationStartv(this, this_rotation)
            this_rotation[3] = -4.61
            setInterpolationRotationEndv(this, this_rotation)
            setInterpolationEnable(this, 1)
            setSpeed(this, 20.0)
            state = 3
            interpTime = 0.0
        end
    elseif state == 3 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)
        
        if interpTime >= 1.0 then
            this_rotation = getRotation(this)
            setInterpolationVariableBeginValue(this, 0.0)
            setInterpolationVariableEndValue(this, 0.5)
            setInterpolationVariableCurrentValue(this, 0.0)
            setInterpolationRotationStartv(this, this_rotation)
            this_rotation[3] = -6.28
            setInterpolationRotationEndv(this, this_rotation)
            setInterpolationEnable(this, 1)
            setSpeed(this, 10.0)
            state = 4
            interpTime = 0.0
        end
    elseif state == 4 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)

        if interpTime >= 0.5 then
            setInterpolationEnable(this, 0)
            setSpeed(this, 10.0)
            state = 5
        end
    end

--    if plr_pos[2] < (this_pos[2] + .5) and 
 --      plr_pos[2] > (this_pos[2] - .5) and 
    if in_view == 1 and state > 0 then
        attemptShot(this, getBoundingSphereRadius(this))
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
