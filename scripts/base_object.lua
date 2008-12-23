function setShot(this, script)
    obj = addObject(this, script)
    radius = getBoundingSphereRadius(this)
    direction = getDirection(this)
    this_position = getPosition(this)

    direction[1] = direction[1] * radius
    direction[2] = direction[2] * radius
    direction[3] = direction[3] * radius

    this_position[1] = this_position[1] + direction[1]
    this_position[2] = this_position[2] + direction[2]
    this_position[3] = this_position[3] + direction[3]

    setPositionv(obj, this_position)

    this_rotation  = getRotation(this)
    this_direction = getDirection(this)
    this_velocity  = getVelocity(this)

    speed = vector_getScalar(this_velocity, this_direction) + 25.0
   
    setRotationv(obj, this_rotation)
    setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

    -- logic here needs to be fixed
    setSpeed(obj, 0, speed)
    return obj
end
