local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "Beam.mdl")
  setRotation(this, 0.0, 0.0, 0.0)
  disableCollisionDetection(this)
  scenery.on_load(this, options)
end
