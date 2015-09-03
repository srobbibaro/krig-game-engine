local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "Pinetree.mdl")
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)

  scale_factor = options.scale_factor
  x_pos        = options.x_pos
  z_pos        = options.z_pos

  if scale_factor then
    krig.object.set_scale(this, scale_factor, scale_factor, scale_factor)
  else
    krig.object.set_scale(this, 2.0, 2.0, 2.0)
    scale_factor = 2.0
  end

  if x_pos and z_pos then
    krig.object.set_position(this, x_pos, 0.0, z_pos)
    krig.object.set_height_from_terrain(this, scale_factor)
  end

  scenery.on_load(this, options)
end
