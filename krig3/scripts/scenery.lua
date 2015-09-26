local game_object = require 'scripts/game_object'

local function on_load(this, options)
  game_object.on_load(this, options)
  this.collision_detection_enabled = false
end

scenery = {
  on_load = on_load
}

return scenery
