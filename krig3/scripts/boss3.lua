dofile('./scripts/base_enemy.lua')

state         = 0
score         = 20000
orig_position = nil
interpTime    = 0.0

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

    -- Disable collision detection when the boss is close to the screen
    -- since it is a little harder to judge for the player
    if (this_pos[3] > 8.0) then
        disableCollisionDetection(this)
    else 
        enableCollisionDetection(this)
    end

    if state == 0 then
        state = 2
        orig_position = getPosition(this)
    elseif state == 1 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)
        
        if interpTime >= 2.0 then
            state = 2
            setInterpolationEnable(this, 0)
        end
    elseif state == 2 then
        interpTime = interpTime + elapsedTime

        if interpTime > 6.0 then
            setSpeed(this, 0.0)

            new_position   = {}
            start_rotation = {}
            end_rotation   = {}

            action = math.random(0,3)
            if action == 0 then 
                -- right to left movement across screen
                new_position = {
                    orig_position[1], 
                    orig_position[2] + math.random(10, 20), 
                    orig_position[3]
                }
                start_rotation = {0.0, -1.5708, -0.8}
                end_rotation = {start_rotation[1], start_rotation[2], -6.0}
            elseif action == 1 then
                -- left to right movement across screen
                new_position = {
                    orig_position[1] - 60.0, 
                    orig_position[2] + math.random(10, 30), 
                    orig_position[3]
                }
                start_rotation = {0.0, 1.5708, 0.8}
                end_rotation = {start_rotation[1], start_rotation[2], 6.0}
            elseif action == 2 then
                -- background to foreground movement across screen
                new_position = {
                    orig_position[1] + math.random(-60, -20), 
                    orig_position[2],
                    orig_position[3] + math.random(-50.0, -30.0)
                }
                start_rotation = {1.37, 3.14, 3.14}
                end_rotation = {-2.0, start_rotation[2], start_rotation[3]}
            else
                -- foreground to background movement across screen
                new_position = {
                    orig_position[1] + math.random(-60, -20), 
                    orig_position[2] + math.random(15, 35),
                    orig_position[3] + 20.0
                }
                start_rotation = {2.0, 0.0, -3.14}
                end_rotation = {-1.37, start_rotation[2], start_rotation[3]}
            end

            -- set orientation and speed
            setPositionv(this, new_position) 
            setRotationv(this, start_rotation)
            setSpeed(this, 45.0 + math.random(0, 15))

            -- setup interpolation
            setInterpolationEnable(this, 1)
            setInterpolationVariableBeginValue(this, 0.0)
            setInterpolationVariableEndValue(this, 2.0)
            setInterpolationVariableCurrentValue(this, 0.0)
            setInterpolationRotationStartv(this, start_rotation)
            setInterpolationRotationEndv(this, end_rotation)
            interpTime = 0.0

            -- transition to active interpolation state
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