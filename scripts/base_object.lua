function setShot(this, script, dir_offset, up_offset)
    if up_offset == nil then up_offset = 0 end
    if dir_offset == nil then dir_offset = 0 end

    obj = addObject(this, script)
    direction = getDirection(this)
    up = getUp(this)
    this_position = getPosition(this)

    direction[1] = direction[1] * dir_offset
    direction[2] = direction[2] * dir_offset
    direction[3] = direction[3] * dir_offset

    up[1] = up[1] * up_offset
    up[2] = up[2] * up_offset
    up[3] = up[3] * up_offset

    this_position[1] = this_position[1] + direction[1] + up[1]
    this_position[2] = this_position[2] + direction[2] + up[2]
    this_position[3] = this_position[3] + direction[3] + up[3]

    setPositionv(obj, this_position)

    this_rotation  = getRotation(this)
    this_direction = getDirection(this)
    this_velocity  = getVelocity(this)

    speed = vector_getScalar(this_velocity, this_direction) + 25.0
   
    setRotationv(obj, this_rotation)
    setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

    -- logic here needs to be fixed
    setSpeed(obj, speed, 0.0, 0.0)
    return obj
end
