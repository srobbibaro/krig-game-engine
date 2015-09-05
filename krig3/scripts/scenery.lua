local game_object = require 'scripts/game_object'

local function on_load(this, options)
  game_object.on_load(this, options)
  krig.object.disable_collision_detection(this)
end

scenery = {
  on_load = on_load
}

return scenery
