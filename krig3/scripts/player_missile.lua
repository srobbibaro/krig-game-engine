-- Configuration
duration = 0.0
obj      = nil
changed  = 0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Missle.mdl")
  setTypeId(this, 2)
  enableAlwaysLit(this)

  playSound(this, "laser.wav")

  obj = level_findObjectOfType(3)
  setRotationVelocity(this, 0.0, 0.0, 0.0)
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

    obj_pos = getPosition(obj)
    obj_radius = getBoundingSphereRadius(obj)
    this_pos = getPosition(this)

    if obj_pos[2] + obj_radius < this_pos[2] then
      this_rot_vel[3] = -30.0 * elapsedTime
      this_rot_vel[2] = 0.0
      this_rot_vel[1] = 0.0
      this_speed[1]   = 20.0
      changed         = 1
    elseif obj_pos[2] - obj_radius > this_pos[2] then
      this_rot_vel[3] = 30.0 * elapsedTime
      this_rot_vel[2] = 0.0
      this_rot_vel[1] = 0.0
      this_speed[1]   = 20.0
      changed         = 2
    else
      this_rot_vel  = { this_dir[1] * 8.0, this_dir[2] * 8.0, this_dir[3] * 8.0 }
      this_speed[1] = this_speed[1] + 8.0 * elapsedTime
      changed       = 0
    end
  else
    obj           = level_findObjectOfType(3)
    this_rot_vel  = { this_dir[1] * 8.0, this_dir[2] * 8.0, this_dir[3] * 8.0 }
    this_speed[1] = this_speed[1] + 8.0 * elapsedTime
    changed       = 0
  end

  setRotationVelocityv(this, this_rot_vel)
  setSpeedv(this, this_speed)
end

function on_collision(this, temp)
  typeId = getTypeId(temp)

  if typeId == 3 then
    removeObject(this)
    duration = 0.0
  end
end

function on_unload(this) end
