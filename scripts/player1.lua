progress = 0

nextShot = 0.0
nextMissileShot = 0.0

leftDown = 0
rightDown = 0
upDown = 0
downDown = 0

function on_load(this)
    -- set default values
    setRotation(this, 0.0, 1.57, 0.0)
    setScale(this, 2.0, 2.0, 2.0)

    -- set test zone enable false
    setVelocity(this, 40.0, 0.0, 0.0)
    setRotationVelocity(this, -10.0, 0.0, 0.0)
    suspend(this, 0.25)

    --obj = this
    return
end

function on_update(this, elapsedTime)
    if nextShot > 0.0 then nextShot = nextShot - elapsedTime end
    if nextMissileShot > 0.0 then nextMissileShot = nextMissileShot - elapsedTime end

    --stuff = getPosition(obj)

    --print("x=" .. stuff[1] .. " y=" .. stuff[2] .. " z=" .. stuff[3])

    if progress == 0 then
        camera = getCamera()
        this_position = getPosition(this)
        camera_position = getPosition(camera)
        tx = camera_position[1] - 20.0
        if this_position[1] >= tx then
            -- set test zone enable true
            setVelocity(this, 8.0, 0.0, 0.0)
            progress = 1
            suspend(this, 1.0)
	  else
            suspend(this, 0.25)
	  end
    elseif progress == 1 then
        setVelocity(this, 10.0, 0.0, 0.0)
        setRotationVelocity(this, 0.0, 0.0, 0.0)
        progress = 2
    elseif progress == 2 then
        this_rotation = getRotation(this)
        -- timer = getTimer(this)
        -- setInterpolationVariable(this, 0, timer, (timer + 0.5))
        -- setInterpolationRotationStartv(this, this_rotation)
        -- setInterpolationRotationEnd(this, 0.0, 1.57, 0.0)
        -- setInterpolationEnable(this, 1)
        suspend(this, 0.75)
        progress = 3
    elseif progress == 3 then
        setInterpolationEnable(this, 0)
        progress = 4
    elseif progress == 4 then
        this_position = getPosition(this)
        this_velocity = getVelocity(this)
        camera = getCamera()
        camera_velocity = getVelocity(camera)

        this_velocity[1] = camera_velocity[1]
        this_velocity[2] = camera_velocity[2]
       
        if engine_testKeyPressed(this, 101) == 1 then upDown = 1 end
        if engine_testKeyPressed(this, 103) == 1 then downDown = 1 end
        if engine_testKeyPressed(this, 100) == 1 then leftDown = 1 end
        if engine_testKeyPressed(this, 102) == 1 then rightDown = 1 end

	  if engine_testKeyReleased(this, 101) == 1 then upDown = 0 end
        if engine_testKeyReleased(this, 103) == 1 then downDown = 0 end
        if engine_testKeyReleased(this, 100) == 1 then leftDown = 0 end
        if engine_testKeyReleased(this, 102) == 1 then rightDown = 0 end

	  if upDown == 1 then this_velocity[2] = this_velocity[2] + 10 end
        if downDown == 1 then this_velocity[2] = this_velocity[2] - 10 end
        if leftDown == 1 then this_velocity[1] = this_velocity[1] - 10 end
	  if rightDown == 1 then this_velocity[1] = this_velocity[1] + 10 end

        setVelocityv(this, this_velocity)

        if engine_testKeyPressed(this, 32) == 1 and nextShot <= 0.0 then
            obj = addObject(this, "./scripts/player_shot.lua")
            setPositionv(obj, this_position)
            nextShot = .40
        end      

	  if engine_testKeyPressed(this, 109) == 1 and nextMissileShot <= 0.0 then
            obj = addObject(this, "./scripts/player_missile.lua")
            setPosition(obj, this_position)
            nextMissileShot = .75
        end      

    end
    
    return
end

function on_unload(this, temp)
    return
end

function on_collision(this,temp)
    return 
end

