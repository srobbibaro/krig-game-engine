progress = 0
life = 10
lives = 100
nextShot = 0.0
nextMissileShot = 0.0

leftDown = 0
rightDown = 0
upDown = 0
downDown = 0

function on_load(this)
    setModel(this, "Ship.mdl")
    -- set default values
    setRotation(this, 0.0, 3.14, 0.0)
    setScale(this, 2.0, 2.0, 2.0)

    -- set test zone enable false
    setVelocity(this, 0.0, 0.0, -20.0)
   
    --obj = this
    return
end

function on_update(this, elapsedTime)
    if nextShot > 0.0 then nextShot = nextShot - elapsedTime end
    if nextMissileShot > 0.0 then nextMissileShot = nextMissileShot - elapsedTime end

    --stuff = getPosition(obj)

    --print("x=" .. stuff[1] .. " y=" .. stuff[2] .. " z=" .. stuff[3])

            this_position = getPosition(this)
        this_velocity = getVelocity(this)
        camera = getCamera()
        camera_velocity = getVelocity(camera)

        this_velocity[1] = camera_velocity[1]
        this_velocity[2] = camera_velocity[2]
       
        if engine_testKeyPressed(101) == 1 then upDown = 1 end
        if engine_testKeyPressed(103) == 1 then downDown = 1 end
        if engine_testKeyPressed(100) == 1 then leftDown = 1 end
        if engine_testKeyPressed(102) == 1 then rightDown = 1 end

	  if engine_testKeyReleased(101) == 1 then upDown = 0 end
        if engine_testKeyReleased(103) == 1 then downDown = 0 end
        if engine_testKeyReleased(100) == 1 then leftDown = 0 end
        if engine_testKeyReleased(102) == 1 then rightDown = 0 end

	  if upDown == 1 then this_velocity[2] = this_velocity[2] + 10 end
        if downDown == 1 then this_velocity[2] = this_velocity[2] - 10 end
        if leftDown == 1 then this_velocity[1] = this_velocity[1] - 10 end
	  if rightDown == 1 then this_velocity[1] = this_velocity[1] + 10 end

        setVelocityv(this, this_velocity)

        if engine_testKeyPressed(32) == 1 and nextShot <= 0.0 then
            obj = addObject(this, "./scripts/player_shot.lua")
            radius = getBoundingSphereRadius(this)
            direction = getDirection(this)
            direction[1] = direction[1] * radius
            direction[2] = direction[2] * radius
            direction[3] = direction[3] * radius

            this_position[1] = this_position[1] + direction[1]
            this_position[2] = this_position[2] + direction[2]
            this_position[3] = this_position[3] + direction[3]

            obj = addObject(this, "./scripts/player_shot.lua")
            setPosition(obj, this_position[1] - .5, this_position[2], this_position[3])
		    setScale(obj, 2.0, 2.0, 2.0)
		    setSpeed(obj, 0, 100.0)
		    
            obj = addObject(this, "./scripts/player_shot.lua")
		    setPosition(obj, this_position[1] + .5, this_position[2], this_position[3])
		    setScale(obj, 2.0, 2.0, 2.0)
		    setSpeed(obj, 0, 100.0)
            
            nextShot = .40
        end      

        if engine_testKeyPressed(32) == 1 and nextShot <= 0.0 then
            obj = addObject(this, "./scripts/player_shot.lua")
            setPosition(obj, this_position[1] - .5, this_position[2], this_position[3])
		setScale(obj, 2.0, 2.0, 2.0)
		setSpeed(obj, 0, 100.0)


		obj = addObject(this, "./scripts/player_shot.lua")
		setPosition(obj, this_position[1] + .5, this_position[2], this_position[3])
		setScale(obj, 2.0, 2.0, 2.0)
		setSpeed(obj, 0, 100.0)

            nextShot = .40
        end      

	  if engine_testKeyPressed(this, 109) == 1 and nextMissileShot <= 0.0 then
            obj = addObject(this, "./scripts/player_missile.lua")
            setPosition(obj, this_position)
            nextMissileShot = .75
        end      

        
    return
end

function on_unload(this, temp)
    return
end

function on_collision(this,temp)
    tempId = getTypeId(temp)

    if tempId == 1 then
        life = life - 1
        if life < 0 then 
            life = 10
            if lives > 0 then lives = lives - 1 end
        end
    end
    return 
end

