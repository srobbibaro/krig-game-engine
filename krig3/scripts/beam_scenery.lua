local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("Beam.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
  scenery.on_load(this, options)
  this:save()
end
