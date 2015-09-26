local game_object = require 'scripts/game_object'
local enemy       = require 'scripts/enemy'

-- Configuration
enemy.score = 500
enemy.life  = 3

local function on_load(this, options)
  this:set_model("Asteroid.mdl")
  this.scale    = {2.0, 2.0, 2.0}
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
  this.type_id  = 4

  game_object.on_load(this, options)

  this:save()
end

local on_collision = enemy.on_collision

local function is_out_of_view(this, this_position, camera_position)
  if this_position[2] < camera_position[2] - 20 then
    krig.level.remove_object(this)
  end
end

asteroid = {
  on_load        = on_load,
  on_collision   = on_collision,
  is_out_of_view = is_out_of_view
}

return asteroid
