local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state      = 0
local delay      = 0.0
local interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  krig.object.set_model(this, "Enemy.mdl")
  krig.object.set_rotation(this, 0.19625, -2.7475, 0.0)
end

function on_update(this, elapsedTime)
  this_pos = krig.object.get_position(this)
  camera   = krig.get_camera()
  cam_pos  = krig.object.get_position(camera)
  tx       = cam_pos[1] + 70.0

  if state == 0 then
    if tx >= this_pos[1] then
      krig.object.set_speed(this, 10.0, 0.0, 0.0)
      this_dir = krig.object.get_direction(this)
      krig.object.set_rotation_velocity_axis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
      state = 1
      delay = 5.0
    end
  elseif state == 1 then
    delay = delay - elapsedTime
    if delay <= 0 then
      krig.object.set_speed(this, 5.0, 0.0, 0.0)
      state = 2
      delay = 0.0
    end
  elseif state == 2 then
    this_rotation = krig.object.get_rotation(this)
    krig.object.set_interpolation_variable_begin_value(this, 0.0)
    krig.object.set_interpolation_variable_end_value(this, 0.5)
    krig.object.set_interpolation_variable_current_value(this, 0.0)
    krig.object.set_interpolation_rotation_start(this, this_rotation)
    krig.object.set_interpolation_rotation_end(this, -0.785, -3.925, 0.0)
    krig.object.set_interpolation_enable(this, 1)

    krig.object.set_speed(this, 30.0)
    krig.object.set_rotation_velocity(this, 0.0, 0.0, 0.0)
    state      = 3
    interpTime = 0.0
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)
    if interpTime >= 0.5 then
      krig.object.set_interpolation_enable(this, 0)
      state = 4
      delay = 0.0
    end
  end
end

on_collision = enemy_ship.on_collision
