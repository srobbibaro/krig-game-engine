dofile('./scripts/base_object.lua')

progress = 0
life = 10
lives = 3
nextShot = 0.0
nextMissileShot = 0.0

leftDown = 0
rightDown = 0
upDown = 0
downDown = 0

invul = 0.0
collisionRecover = 0
interpTime = 0.0

function on_load(this)
    setModel(this, "Ship.mdl")
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
    if invul > 0.0 then invul = invul - elapsedTime end

    -- handle invul --
    if collisionRecover == 1 then
        isDrawn = getDrawEnabled(this)
        
        if isDrawn == 0 then
            enableDraw(this)
        else
            disableDraw(this)
        end
        
        if invul <= 0.0 then
            enableDraw(this)
            collisionRecover = 0
            enableCollisionDetection(this)
        end
    end

    -- handle individual states
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
        setInterpolationVariableBeginValue(this, 0.0)
        setInterpolationVariableEndValue(this, 0.5)
        setInterpolationVariableCurrentValue(this, 0.0)
        setInterpolationRotationStartv(this, this_rotation)
        setInterpolationRotationEnd(this, 0.0, 1.57, 0.0)
        setInterpolationEnable(this, 1)
        progress = 3
    elseif progress == 3 then
        interpTime = interpTime + elapsedTime
        setInterpolationVariableCurrentValue(this, interpTime)

        if interpTime > 0.5 then
            setInterpolationEnable(this, 0)
            progress = 4
        end
    elseif progress == 4 then
        this_position = getPosition(this)
        this_velocity = getVelocity(this)
        camera = getCamera()
        camera_velocity = getVelocity(camera)

        this_velocity[1] = camera_velocity[1]
        this_velocity[2] = camera_velocity[2]
       
        if engine_testSpecialKeyPressed(101) == 1 then upDown = 1 end
        if engine_testSpecialKeyPressed(103) == 1 then downDown = 1 end
        if engine_testSpecialKeyPressed(100) == 1 then leftDown = 1 end
        if engine_testSpecialKeyPressed(102) == 1 then rightDown = 1 end

	    if engine_testSpecialKeyReleased(101) == 1 then upDown = 0 end
        if engine_testSpecialKeyReleased(103) == 1 then downDown = 0 end
        if engine_testSpecialKeyReleased(100) == 1 then leftDown = 0 end
        if engine_testSpecialKeyReleased(102) == 1 then rightDown = 0 end

	    if upDown == 1 then this_velocity[2] = this_velocity[2] + 10 end
        if downDown == 1 then this_velocity[2] = this_velocity[2] - 10 end
        if leftDown == 1 then this_velocity[1] = this_velocity[1] - 10 end
	    if rightDown == 1 then this_velocity[1] = this_velocity[1] + 10 end

        setVelocityv(this, this_velocity)

        if engine_testKeyPressed(32) == 1 and nextShot <= 0.0 then
            setShot(this, "./scripts/player_shot.lua") 
            nextShot = .40
        end      

	    if engine_testKeyPressed(this, 109) == 1 and nextMissileShot <= 0.0 then
            --obj = addObject(this, "./scripts/player_missile.lua")
            obj = setShot(this, "./scripts/player_shot.lua") 
            nextMissileShot = .75
        end      

    end
    
    return
end

function on_unload(this, temp)
    return
end

function on_collision(this,temp)
    tempId = getTypeId(temp)

    if tempId == 1 or tempId == 10 or tempId == 4 then
        life = life - 1
        collisionRecover = 1
        disableCollisionDetection(this)
        invul = 2.0
        if life <= 0 then 
            life = 10
            if lives > 0 then lives = lives - 1 end
        end
    end
    return 
end

