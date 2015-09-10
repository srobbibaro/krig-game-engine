-- Configuration
local duration = 0.0
local obj      = nil
local changed  = 0

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "Missle.mdl")
  krig.object.set_type_id(this, 2)
  krig.object.enable_always_lit(this)

  krig.play_sound(this, "laser.wav")

  obj = krig.level.find_object_of_type(1)
  krig.object.set_rotation_velocity(this, 0.0, 0.0, 0.0)
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration > 5.0 then
    krig.level.remove_object(this)
    duration = 0.0
  end

  this_dir = krig.object.get_direction(this)
  this_rot_vel = krig.object.get_rotation_velocity(this)
  this_speed = krig.object.get_speed(this)
  this_orth  = krig.object.get_orthogonal(this)

  if obj.id >= 0 then
    krig.object.set_speed(this, 0.0, 0.0, 0.0)

    obj_pos = krig.object.get_position(obj)
    obj_radius = krig.object.get_bounding_sphere_radius(obj)
    this_pos = krig.object.get_position(this)

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
    obj           = krig.level.find_object_of_type(1)
    this_rot_vel  = { this_dir[1] * 8.0, this_dir[2] * 8.0, this_dir[3] * 8.0 }
    this_speed[1] = this_speed[1] + 8.0 * elapsedTime
    changed       = 0
  end

  krig.object.set_rotation_velocity(this, this_rot_vel)
  krig.object.set_speed(this, this_speed)
end

function on_collision(this, temp)
  typeId = krig.object.get_type_id(temp)

  if typeId ~= 0 then
    krig.level.remove_object(this)
    duration = 0.0
  end
end
