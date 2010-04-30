duration = 0.0
obj = nil
changed = 0

function on_load(this)
    setModel(this, "Missle.mdl")
    setTypeId(this, 2)
    enableAlwaysLit(this)

    playSound(this, "laser.wav")

    obj = level_findObjectOfType(3)
    setRotationVelocity(this, 0.0, 0.0, 0.0)

    return
end

function on_update(this, elapsedTime)
    duration = duration + elapsedTime

    if duration > 5.0 then
        removeObject(this)
        duration = 0.0
    end

    this_dir = getDirection(this)
    this_rot_vel = getRotationVelocity(this)
    this_speed = getSpeed(this)
    this_orth  = getOrthogonal(this)

    if obj ~= nil then
    setSpeed(this, 0.0, 0.0, 0.0)

--[[
    obj_up   = getUp(obj)
    result = {obj_up[1] - this_dir[1], obj_up[2] - this_dir[2], obj_up[3] - this_dir[3]}

       v1 = vector_dotProduct(obj_up, this_dir)
       v2 = vector_dotProduct(obj_up, result)

        ratio = v2 / v1

        final = { this_dir[1] * ratio, this_dir[2] * ratio, this_dir[3] * ratio }

        print ("final x= "..final[1].." y="..final[2].." z="..final[3])
--]]
        obj_pos = getPosition(obj)
        obj_radius = getBoundingSphereRadius(obj)
        this_pos = getPosition(this)

        if obj_pos[2] + obj_radius < this_pos[2] then
            this_rot_vel[3] = -30.0 * elapsedTime
            this_rot_vel[2] = 0.0
            this_rot_vel[1] = 0.0
            this_speed[1] = 20.0
            changed = 1
        elseif obj_pos[2] - obj_radius > this_pos[2] then
            this_rot_vel[3] = 30.0 * elapsedTime
            this_rot_vel[2] = 0.0
            this_rot_vel[1] = 0.0
            this_speed[1] = 20.0
            changed = 2
        else
            this_rot_vel = { this_dir[1] * 8.0, this_dir[2] * 8.0, this_dir[3] * 8.0 }
            this_speed[1] = this_speed[1] + 8.0 * elapsedTime
            changed = 0
        end
    else
        obj = level_findObjectOfType(3)
        this_rot_vel = { this_dir[1] * 8.0, this_dir[2] * 8.0, this_dir[3] * 8.0 }
        this_speed[1] = this_speed[1] + 8.0 * elapsedTime
        changed = 0
    end

    setRotationVelocityv(this, this_rot_vel)
    setSpeedv(this, this_speed)
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    typeId = getTypeId(temp)

    if typeId == 3 then
        removeObject(this)
        duration = 0.0
    end
    return 
end
