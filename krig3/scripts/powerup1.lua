local powerup = require 'scripts/powerup'

-- Configuration
powerup.max_duration = 7.0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("Powerup.mdl")
  this.scale             = {2.0, 2.0, 2.0}
  this.rotation_velocity = {0.0, 2.0, 0.0}
  this.type_id           = 5
  this:save()
end

on_update    = powerup.on_update
on_collision = powerup.on_collision
