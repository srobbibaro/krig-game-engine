x_min = 0.0
x_max = 0.0
y_min = 0.0
y_max = 0.0

life = 40

shot_timer = 0.0

function set_window()
    camera = getCamera()
    cx, cy, cz = getPosition(camera)
    x_min = cx - math.random(20)
    x_max = cx + math.random(20)
    y_min = cy - math.random(20)
    y_max = cy + math.random(20)

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
    return   
end

function on_update(this, elapsedTime)
    mx, my, mz = getPosition(this)  
    vx, vy, vz = getVelocity(this)

    shot_timer = shot_timer + elapsedTime
    if shot_timer > 2.0 then
        shot_timer = 0.0
        fire_shot(this)
    end

    if vx > 0.0 then
        if mx > x_max then
            speed = calc_speed()
            setVelocity(this, 0.0, -speed, 0.0)
        end
    elseif vx < 0.0 then
        if mx < x_min then
	      speed = calc_speed()
            setVelocity(this, 0.0, speed, 0.0)
        end
    elseif vy > 0.0 then
        if my > y_max then
            speed = calc_speed()
            setVelocity(this, speed, 0.0, 0.0)
        end
    elseif vy < 0.0 then
        if my < y_min then
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
    playSound(this, 3)
    life = life - 1
    if life <= 0 then
        removeObject(this)
    end
    return
end

function fire_shot(this)
    print ("shooting...\n")
    return
end

