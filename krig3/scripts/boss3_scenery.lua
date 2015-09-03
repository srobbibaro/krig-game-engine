-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "FishBoss.mdl")
  krig.object.set_scale(this, 16.0, 16.0, 16.0)
  krig.object.set_rotation(this, 0.0, 1.5708, 1.2)

  krig.object.set_type_id(this, 3)

  krig.object.disable_collision_detection(this)
end

function on_update(this, elapsedTime)
  this_pos = krig.object.get_position(this)
  camera   = krig.get_camera()
  cam_pos  = krig.object.get_position(camera)

  if state == 0 then
    if cam_pos[1] - 50.0 > this_pos[1] then
      this_rotation = krig.object.get_rotation(this)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 4.0)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, this_rotation)

      this_rotation[3] = -2.0
      krig.object.set_interpolation_rotation_end(this, this_rotation)
      krig.object.set_interpolation_enable(this, 1)

      krig.object.set_speed(this, 40.0)
      state      = 1
      interpTime = 0.0
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 4.0 then
      state = 2
    end
  elseif state == 2 then
    if this_pos[2] < -10.0 then
      krig.level.remove_object(this)
    end
  end
end
