progress = 0

nextShot = 0.0

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
    return
end

function on_update(this, elapsedTime)
    if nextShot > 0.0 then nextShot = nextShot - elapsedTime end

    if progress == 0 then
        camera = getCamera()
        mx, my, mz = getPosition(this)
        cx, cy, cz = getPosition(camera)
        tx = cx - 20.0
        if mx >= tx then
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
        rx, ry, rz = getRotation(this)
        -- timer = getTimer(this)
        -- setInterpolationVariable(this, 0, timer, (timer + 0.5))
        -- setInterpolationRotationStart(this, rx, ry, rz)
        -- setInterpolationRotationEnd(this, 0.0, 1.57, 0.0)
        -- setInterpolationEnable(this, 1)
        suspend(this, 0.75)
        progress = 3
    elseif progress == 3 then
        setInterpolationEnable(this, 0)
        progress = 4
    elseif progress == 4 then
        px, py, pz = getPosition(this)
        pvx, pvy, pvz = getVelocity(this)
        camera = getCamera()
        cvx, cvy, cvz = getVelocity(camera)

        pvx = cvx
        pvy = cvy

        if engine_testKeyPressed(this, 101) == 1 then upDown = 1 end
        if engine_testKeyPressed(this, 103) == 1 then downDown = 1 end
        if engine_testKeyPressed(this, 100) == 1 then leftDown = 1 end
        if engine_testKeyPressed(this, 102) == 1 then rightDown = 1 end

	  if engine_testKeyReleased(this, 101) == 1 then upDown = 0 end
        if engine_testKeyReleased(this, 103) == 1 then downDown = 0 end
        if engine_testKeyReleased(this, 100) == 1 then leftDown = 0 end
        if engine_testKeyReleased(this, 102) == 1 then rightDown = 0 end

	  if upDown == 1 then pvy = pvy + 10 end
        if downDown == 1 then pvy = pvy - 10 end
        if leftDown == 1 then pvx = pvx - 10 end
	  if rightDown == 1 then pvx = pvx + 10 end

        setVelocity(this, pvx, pvy, pvz)

        if engine_testKeyPressed(this, 32) == 1 and nextShot <= 0.0 then
            obj = addObject(this, "./scripts/player_shot.lua")
            setPosition(obj, px, py, pz)
            nextShot = .40
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

