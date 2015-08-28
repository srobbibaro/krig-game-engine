local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "Pinetree.mdl")
  setRotation(this, 0.0, 0.0, 0.0)

  scale_factor = options.scale_factor
  x_pos        = options.x_pos
  z_pos        = options.z_pos

  if scale_factor then
    setScale(this, scale_factor, scale_factor, scale_factor)
  else
    setScale(this, 2.0, 2.0, 2.0)
    scale_factor = 2.0
  end

  if x_pos and z_pos then
    setPosition(this, x_pos, 0.0, z_pos)
    setHeightFromTerrain(this, scale_factor)
  end

  scenery.on_load(this, options)
end
