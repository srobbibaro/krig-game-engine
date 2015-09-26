local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, {velocity = {10.0, 0.0, 0.0}})
  this:save()
end
