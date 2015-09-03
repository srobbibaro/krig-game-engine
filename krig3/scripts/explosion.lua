local game_object = require 'scripts/game_object'

-- Configuration
local duration = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "Explosion.mdl")
  krig.object.set_scale_rate(this, 1.0, 1.0, 1.0)
  krig.object.disable_collision_detection(this)
  krig.object.enable_always_lit(this)
  game_object.on_load(this, options)
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration >= 2.0 then
    krig.level.remove_object(this)
  end
end
