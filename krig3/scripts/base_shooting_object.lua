-- Configuration
next_shot_time         = 1.0
initial_next_shot_time = 1.0
script                 = nil

-- Helper Functions
function setupShots(this, objectScript, initialNextShotTime)
  script                 = objectScript
  initial_next_shot_time = initialNextShotTime
  next_shot_time         = initial_next_shot_time
end

function setShot(this, script, dir_offset, up_offset)
  if up_offset == nil then up_offset = 0 end
  if dir_offset == nil then dir_offset = 0 end

  obj           = addObject(this, script)
  direction     = getDirection(this)
  up            = getUp(this)
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

  speed = 20.0

  setRotationv(obj, this_rotation)
  setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

  -- TODO: logic here needs to be fixed
  setSpeed(obj, speed, 0.0, 0.0)

  return obj
end

function setShotNew(this, obj, dir_offset, up_offset)
  if up_offset == nil then up_offset = 0 end
  if dir_offset == nil then dir_offset = 0 end

  direction     = getDirection(this)
  up            = getUp(this)
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

  speed = 25

  setRotationv(obj, this_rotation)
  setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

  -- TODO: logic here needs to be fixed
  setSpeed(obj, speed, 0.0, 0.0)

  return obj
end

function setShotWithDirectionNew(this, obj, direction, dir_offset, up_offset)
  if up_offset == nil then up_offset = 0 end
  if dir_offset == nil then dir_offset = 0 end

  up            = getUp(this)
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

  speed = 25

  setRotationv(obj, this_rotation)
  setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

  -- TODO: logic here needs to be fixed
  setSpeed(obj, speed, 0.0, 0.0)

  return obj
end

function update_shots(elapsedTime)
  next_shot_time = next_shot_time - elapsedTime
end

function attemptShot(this, dir_offset, up_offset)
  if next_shot_time <= 0.0 then
    shot = addObject(this, script)
    setShotNew(this, shot, dir_offset, up_offset)
    next_shot_time = initial_next_shot_time
  end
end

function attemptShotWithDirection(this, direction, dir_offset, up_offset)
  if next_shot_time <= 0.0 then
    shot = addObject(this, script)
    setShotWithDirectionNew(this, shot, direction, dir_offset, up_offset)
    next_shot_time = initial_next_shot_time
  end
end

function attemptShots(this, num_shots, dir_offset, up_offset)
  if num_shots == nil then num_shots = 1 end

  if next_shot_time <= 0.0 then
    for i = 1, num_shots do
      shot = addObject(this, script)
      setShotNew(this, shot, dir_offset[i], up_offset[i])
    end
    next_shot_time = initial_next_shot_time
  end
end
