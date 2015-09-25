local game_object = require 'scripts/game_object'
local enemy       = require 'scripts/enemy'

-- Configuration
enemy.score = 100

-- Overridden Engine Callbacks
local function on_load(this, options)
  game_object.on_load(this, options)
  this:set_model("Enemy.mdl")
  this.scale    = {2.0, 2.0, 2.0}
  this.rotation = krig.rotation.from_euler({0.0, -1.57, 0.0})
  this.type_id  = 1
  this:save()
end

enemy_ship = {
  on_load      = on_load,
  on_collision = enemy.on_collision
}

return enemy_ship
