local game_object = require 'scripts/game_object'
local enemy       = require 'scripts/enemy'

-- Configuration
enemy.score = 500
enemy.life  = 3

local function on_load(this, options)
  setModel(this, "Asteroid.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotation(this, 0.0, 0.0, 0.0)

  setTypeId(this, 4)
  game_object.on_load(this, options)
end

local on_collision = enemy.on_collision

local function is_out_of_view(this, this_position, camera_position)
  if this_position[2] < camera_position[2] - 20 then
    removeObject(this)
  end
end

asteroid = {
  on_load        = on_load,
  on_collision   = on_collision,
  is_out_of_view = is_out_of_view
}

return asteroid
