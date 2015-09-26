local enemy = require 'scripts/enemy'
local game_object = require 'scripts/game_object'

-- Configuration
enemy.score = 500

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("redrock.mdl")
  this.scale    = {1.0, 1.0, 1.0}
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
  this.type_id  = 4

  game_object.on_load(this, options)

  this:save()
end
