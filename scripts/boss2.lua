x_min = 0.0
x_max = 0.0
y_min = 0.0
y_max = 0.0

life = 40

shot_timer = 0.0

function set_window()
    camera = getCamera()
    camera_position = getPosition(camera)
    x_min = camera_position[1] - math.random(20)
    x_max = camera_position[1] + math.random(20)
    y_min = camera_position[2] - math.random(20)
    y_max = camera_position[2] + math.random(20)

    return
end

function calc_speed()
    speed = math.random(20)
    speed = 40.0 - speed
    return speed
end

function on_load(this)
    setModel(this, "snowboss.mdl")
    setRotation(this, 0.0, -1.57, 0.0)
    setScale(this, 4.0, 4.0, 4.0)

    math.randomseed( os.time() )
    set_window()
    speed = calc_speed()
    setVelocity(this, -speed, 0.0, 0.0)
    setTypeId(this, 1)
    return   
end

function on_update(this, elapsedTime)
    this_position = getPosition(this)  
    this_velocity = getVelocity(this)

    shot_timer = shot_timer + elapsedTime
    if shot_timer > 2.0 then
        shot_timer = 0.0
        fire_shot(this)
    end

    if this_velocity[1] > 0.0 then
        if this_position[1] > x_max then
            speed = calc_speed()
            setVelocity(this, 0.0, -speed, 0.0)
        end
    elseif this_velocity[1] < 0.0 then
        if this_position[1] < x_min then
	      speed = calc_speed()
            setVelocity(this, 0.0, speed, 0.0)
        end
    elseif this_velocity[2] > 0.0 then
        if this_position[2] > y_max then
            speed = calc_speed()
            setVelocity(this, speed, 0.0, 0.0)
        end
    elseif this_velocity[2] < 0.0 then
        if this_position[2] < y_min then
            speed = calc_speed()
            setVelocity(this, -speed, 0.0, 0.0)
            set_window()
        end
    end

    return    
end

function on_unload(this)
    return
end

function on_collision(this, temp)
     typeId = getTypeId(temp)

     if typeId == 2 then
        life = life - 1
        playSound(this, "explosion1.wav")

        if life <= 0 then
            this_position = getPosition(this)

            obj = addObject(this, "./scripts/explosion.lua")
            setPosition(obj, this_position[1], this_position[2], this_position[3])
            removeObject(this)
        end
    end

    return
end

function fire_shot(this)
    return
end

