local game_object = require 'scripts/game_object'

-- Configuration
local duration = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("Explosion.mdl")
  this.scale_rate                  = {1.0, 1.0, 1.0}
  this.collision_detection_enabled = false
  this.always_lit                  = true
  game_object.on_load(this, options)
  this:save()
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration >= 2.0 then
    krig.level.remove_object(this)
  end
end
