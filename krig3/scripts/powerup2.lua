local powerup = require 'scripts/powerup'

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "Powerup2.mdl")
  krig.object.set_scale(this, 2.0, 2.0, 2.0)
  krig.object.set_rotation_velocity(this, 0.0, 2.0, 0.0);
  krig.object.set_type_id(this, 6);
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
