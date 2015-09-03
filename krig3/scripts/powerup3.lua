local powerup = require 'scripts/powerup'

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "Ship.mdl")
  krig.object.set_rotation_velocity(this, 0.0, 2.0, 0.0);
  krig.object.set_type_id(this, 7);
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
