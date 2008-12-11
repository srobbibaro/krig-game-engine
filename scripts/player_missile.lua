duration = 0.0

function on_load(this)
    setModel(this, "Missle.mdl")

    player = getPlayer()
    player_rotation  = getRotation(player)
    player_direction = getDirection(player)
    player_velocity  = getVelocity(player)

    speed = vector_getScalar(player_velocity, player_direction) + 25.0

    setRotation(this, player_rotation[1], player_rotation[2], player_rotation[3])
    
    -- logic here needs to be fixed
    setSpeed(this, 0, speed)

    playSound(this, "explosion1.wav")

    return
end

function on_update(this, elapsedTime)
    duration = duration + elapsedTime

    if duration > 150.0 then
        removeObject(this)   
    end
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
   -- removeObject(this)
    return 
end
