dofile('./scripts/base_enemy.lua')

state = 0
score = 20000
orig_position = nil

function on_load(this)
    setModel(this, "FishBoss.mdl")
    setScale(this, 16.0, 16.0, 16.0)
    setRotation(this, 0.0, -1.5708, -1.2)

    life = 40
    setTypeId(this, 1)
    
    return
end

function on_update(this, elapsedTime)
    this_pos = getPosition(this)
    camera = getCamera()
    cam_pos = getPosition(camera)

    if state == 0 then
        state = 1
        orig_position = getPosition(this)
    elseif state == 1 then
        this_rotation = getRotation(this)
        setInterpolationVariableBeginValue(this, 0.0)
        setInterpolationVariableEndValue(this, 2.0)
        setInterpolationVariableCurrentValue(this, 0.0)
        setInterpolationRotationStartv(this, this_rotation)
        this_rotation[3] = 2
        setInterpolationRotationEndv(this, this_rotation)
        setInterpolationEnable(this, 1)
        setSpeed(this, 60.0)
        state = 2
        interpTime = 0.0
    elseif state == 2 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)
        
        if interpTime >= 2.0 then
            state = 3
            setInterpolationEnable(this, 0)
        end
    elseif state == 3 then
        interpTime = interpTime + elapsedTime
        if interpTime > 6.0 then
            setSpeed(this, 0.0)
            setPositionv(this, orig_position) 
            setRotation(this, 0.0, -1.5708, -1.2)
            state = 1
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
