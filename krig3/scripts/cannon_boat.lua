local boat = require 'scripts/boat'

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "BoatCannon.mdl")
  krig.object.set_rotation(this, 0.0, -1.57, 0.0)
  krig.object.disable_collision_detection(this)
  krig.object.set_scale(this, 10.0, 10.0, 10.0)
end

function on_update(this, elapsedTime)
  krig.object.orient_on_terrain(this, 0.0, 0.0, 0.0)
end

on_collision = boat.on_collision
