local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "Building.mdl")
  krig.object.set_rotation(this, 0.0, 1.0, 0.0)
  scenery.on_load(this, options)
end
