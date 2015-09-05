local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
local state      = 0
local interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.5)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this_pos = krig.object.get_position(this)
  camera   = krig.get_camera()
  cam_pos  = krig.object.get_position(camera)
  in_view  = krig.object.get_in_view(this)
  player   = krig.get_player()
  plr_pos  = krig.object.get_position(player)

  if state == 0 then
    if cam_pos[1] + 10.0 > this_pos[1] then
      this_rotation = krig.object.get_rotation(this)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 0.5)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, this_rotation)

      this_rotation[3] = 1.57
      krig.object.set_interpolation_rotation_end(this, this_rotation)
      krig.object.set_interpolation_enable(this, 1)

      krig.object.set_speed(this, 10.0)
      state      = 1
      interpTime = 0.0
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 0.5 then
      this_rotation = krig.object.get_rotation(this)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 1.0)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, this_rotation)

      this_rotation[3] = 3.14
      krig.object.set_interpolation_rotation_end(this, this_rotation)
      krig.object.set_interpolation_enable(this, 1)

      krig.object.set_speed(this, 20.0)
      state      = 2
      interpTime = 0.0
    end
  elseif state == 2 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 1.0 then
      this_rotation = krig.object.get_rotation(this)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 1.0)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, this_rotation)

      this_rotation[3] = 4.61
      krig.object.set_interpolation_rotation_end(this, this_rotation)
      krig.object.set_interpolation_enable(this, 1)

      krig.object.set_speed(this, 20.0)
      state      = 3
      interpTime = 0.0
    end
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 1.0 then
      this_rotation = krig.object.get_rotation(this)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 0.5)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, this_rotation)

      this_rotation[3] = 6.28
      krig.object.set_interpolation_rotation_end(this, this_rotation)
      krig.object.set_interpolation_enable(this, 1)

      krig.object.set_speed(this, 10.0)
      state      = 4
      interpTime = 0.0
    end
  elseif state == 4 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 0.5 then
      krig.object.set_interpolation_enable(this, 0)
      krig.object.set_speed(this, 10.0)
      state = 5
    end
  end

  if in_view == 1 and state > 0 then
    shooting_object.attemptShot(this, krig.object.get_bounding_sphere_radius(this))
  end
end

on_collision = enemy_ship.on_collision
