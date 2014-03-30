dofile('./scripts/base_enemy.lua')

score = 100
state = 0

function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.19625, -2.7475, 0.0)
    setTypeId(this, 1)   
    return
end

function on_update(this, elapsedTime)
    this_pos = getPosition(this)
    camera = getCamera()
    cam_pos = getPosition(camera)
    tx = cam_pos[1] + 70.0
    if tx >= this_pos[1] and state == 0 then
        setSpeed(this, 20.0, 0.0, 0.0)
        this_dir = getDirection(this)
        setRotationVelocityAxis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
        state = 1
    end
	return
end

function on_unload(this)
	return
end

function on_collision(this, temp)
    this_id = getTypeId(this)
    temp_id = getTypeId(temp)

    if temp_id == 100 then
        this_pos = getPosition(this)
        this_radius = getBoundingSphereRadius(this)
        terrain_height = terrain_getHeight(this_pos[1], this_pos[3])

        if (this_pos[2] - this_radius) <= terrain_height then
            playSound(this, "explosion.wav")

            create_explosion(this)
            removeObject(this)
        end
    else
        handle_collision(this, temp)
    end

    return
end
