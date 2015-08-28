local powerup = require 'scripts/powerup'

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Powerup2.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotationVelocity(this, 0.0, 2.0, 0.0);
  setTypeId(this, 6);
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
