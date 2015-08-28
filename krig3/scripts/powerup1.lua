local powerup = require 'scripts/powerup'

-- Configuration
powerup.max_duration = 7.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Powerup.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotationVelocity(this, 0.0, 2.0, 0.0);
  setTypeId(this, 5);
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
