local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "Beam.mdl")
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)
  krig.object.disable_collision_detection(this)
  scenery.on_load(this, options)
end
