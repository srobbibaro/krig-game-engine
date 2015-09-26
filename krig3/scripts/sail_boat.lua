local boat    = require 'scripts/boat'
local scenery = require 'scripts/scenery'

-- Overridden Engine Callbacks
function on_load(this, options)
  scenery.on_load(this, options)
  this:set_model("SailBoat.mdl")
  this.rotation = krig.rotation.from_euler({0.0, -1.57, 0.0})
  this.scale    = {7.0, 7.0, 7.0}
  this:save()
end

function on_update(this, elapsedTime)
  this:orient_on_terrain(0.0, 3.14, 0.0)
end

on_collision = boat.on_collision
