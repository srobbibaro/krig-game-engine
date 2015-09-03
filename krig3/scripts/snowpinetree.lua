local scenery = require 'scripts/scenery'

-- Configuration
state        = 0
scale_factor = 0.0

-- Overridden Engine Callbacks
function on_load(this, x_pos, z_pos, temp_scale_factor)
  krig.object.set_model(this, "Snowpine.mdl")
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)

  if temp_scale_factor ~= nil then
    scale_factor = temp_scale_factor
    krig.object.set_scale(this, scale_factor, scale_factor, scale_factor)
  else
    scale_factor = 2.0
    krig.object.set_scale(this, scale_factor, scale_factor, scale_factor)
  end

  if x_pos ~= nil and z_pos ~= nil then
    krig.object.set_position(this, x_pos, 0.0, z_pos)
    krig.object.set_height_from_terrain(this, scale_factor)
  end

  scenery.on_load(this, options)
end

function on_update(this, elapsedTime)
  in_view = krig.object.get_in_view(this)

  if state == 0 and in_view == 1 then
    state = 1
  elseif state == 1 and in_view == 0 then
    state = 0

    this_pos = krig.object.get_position(this)
    plane = krig.camera.get_frustum_plane(1)

    x = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])
    -- Ax + By + Cz + D = 0

    krig.object.set_position(this, x + math.random(50), 0.0, this_pos[3])
    krig.object.set_height_from_terrain(this, scale_factor)
  end
end
