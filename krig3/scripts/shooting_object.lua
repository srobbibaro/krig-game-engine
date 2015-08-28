local function setupShots(this, objectScript, initialNextShotTime)
  shooting_object.script                 = objectScript
  shooting_object.initial_next_shot_time = initialNextShotTime
  shooting_object.next_shot_time         = initialNextShotTime
end

local function setShot(this, script, dir_offset, up_offset)
  if up_offset == nil then up_offset = 0 end
  if dir_offset == nil then dir_offset = 0 end

  local obj           = addObject(script)
  local direction     = getDirection(this)
  local up            = getUp(this)
  local this_position = getPosition(this)
  local speed         = 20.0

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

  setRotationv(obj, this_rotation)
  setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

  -- TODO: logic here needs to be fixed
  setSpeed(obj, speed, 0.0, 0.0)

  return obj
end

local function update_shots(elapsedTime)
  shooting_object.next_shot_time = shooting_object.next_shot_time - elapsedTime
end

local function attemptShot(this, dir_offset, up_offset)
  if shooting_object.next_shot_time <= 0.0 then
    shot = addObject(shooting_object.script)
    setShotNew(this, shot, dir_offset, up_offset)
    shooting_object.next_shot_time = shooting_object.initial_next_shot_time
  end
end

local function attemptShots(this, num_shots, dir_offset, up_offset)
  if num_shots == nil then num_shots = 1 end

  if shooting_object.next_shot_time <= 0.0 then
    for i = 1, num_shots do
      shot = addObject(shooting_object.script)
      setShotNew(this, shot, dir_offset[i], up_offset[i])
    end
    shooting_object.next_shot_time = shooting_object.initial_next_shot_time
  end
end

function setShotNew(this, obj, dir_offset, up_offset)
  if up_offset == nil then up_offset = 0 end
  if dir_offset == nil then dir_offset = 0 end

  local direction     = getDirection(this)
  local up            = getUp(this)
  local this_position = getPosition(this)
  local speed         = 25

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

  setRotationv(obj, this_rotation)
  setRotationVelocity(obj, this_direction[1] * 8.0, this_direction[2] * 8.0, this_direction[3] * 8.0)

  -- TODO: logic here needs to be fixed
  setSpeed(obj, speed, 0.0, 0.0)

  return obj
end

shooting_object = {
  setupShots             = setupShots,
  setShot                = setShot,
  update_shots           = update_shots,
  attemptShot            = attemptShot,
  attemptShots           = attemptShots,
  next_shot_time         = 1.0,
  initial_next_shot_time = 1.0,
  script                 = nil
}

return shooting_object
