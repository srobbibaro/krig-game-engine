state = 0
scale_factor = 0.0

function on_load(this, x_pos, z_pos, temp_scale_factor)
    setModel(this, "Snowpine.mdl")
    setRotation(this, 0.0, 0.0, 0.0)     

    if temp_scale_factor ~= nil then
        scale_factor = temp_scale_factor
        setScale(this, scale_factor, scale_factor, scale_factor)    
    else
        scale_factor = 2.0
        setScale(this, scale_factor, scale_factor, scale_factor)    
    end

    if x_pos ~= nil and z_pos ~= nil then
        setPosition(this, x_pos, 0.0, z_pos)
        setHeightFromTerrain(this, scale_factor)
    end    

    disableCollisionDetection(this)
    return
end

function on_update(this, elapsedTime)
    in_view = getInView(this)

    if state == 0 and in_view == 1 then
        state = 1
    elseif state == 1 and in_view == 0 then
        state = 0

        this_pos = getPosition(this)
        plane = camera_getFrustumPlane(1)

        x = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])
        --Ax + By + Cz + D = 0

        setPosition(this, x + math.random(50), 0.0, this_pos[3])
        setHeightFromTerrain(this, scale_factor)
    end
    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    return
end
