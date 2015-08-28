local game_object = require 'scripts/game_object'
local enemy = require 'scripts/enemy'

-- Configuration
enemy.score = 100

-- Overridden Engine Callbacks
local function on_load(this, options)
  game_object.on_load(this, options)
  setModel(this, "Enemy.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotation(this, 0.0, -1.57, 0.0)
  setTypeId(this, 1)
end

enemy_ship = {
  on_load      = on_load,
  on_collision = enemy.on_collision
}

return enemy_ship
