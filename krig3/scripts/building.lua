local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "Building.mdl")
  setRotation(this, 0.0, 1.0, 0.0)
  scenery.on_load(this, options)
end
