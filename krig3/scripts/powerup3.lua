local powerup = require 'scripts/powerup'

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Ship.mdl")
  setRotationVelocity(this, 0.0, 2.0, 0.0);
  setTypeId(this, 7);
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
