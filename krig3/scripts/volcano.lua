-- Configuration
local shake = 0

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "volcano.mdl")
  krig.object.set_scale(this, 20.0, 20.0, 20.0)
  krig.object.set_rotation(this, 0.0, 1.0, 0.0)
  krig.object.disable_collision_detection(this)
end

function on_update(this, elapsedTime)
  local camera          = krig.get_camera()
  local this_position   = krig.object.get_position(this)
  local camera_position = krig.object.get_position(camera)

  -- Don't start shaking until we're close to it.
  if camera_position[1] < this_position[1]-30.0 or
     camera_position[1] > this_position[1]+30.0 then
    return
  end

  -- We're in range, shake it baby.
  if shake == 0 then
    krig.object.set_scale(this, 21.0, 21.0, 21.0);
    shake = 1
  else
    krig.object.set_scale(this, 20.0, 20.0, 20.0);
    shake = 0
  end
end
