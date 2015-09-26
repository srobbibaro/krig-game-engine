local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("Pinetree.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})

  scale_factor = options.scale_factor or 2.0

  this.scale = {scale_factor, scale_factor, scale_factor}

  if options.x_pos and options.z_pos then
    this.position = {options.x_pos, 0.0, options.z_pos}
  end

  scenery.on_load(this, options)
  this:save()

  this:set_height_from_terrain(scale_factor)
end
