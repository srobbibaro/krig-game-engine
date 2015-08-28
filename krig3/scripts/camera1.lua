local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this)
  game_object.on_load(this, {velocity = {10.0, 0.0, 0.0}})
end
