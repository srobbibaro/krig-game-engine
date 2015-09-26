local boat        = require 'scripts/boat'
local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)
  this:set_model("BoatCannon.mdl")
  this.rotation                    = krig.rotation.from_euler({0.0, -1.57, 0.0})
  this.collision_detection_enabled = false
  this.scale                       = {10.0, 10.0, 10.0}
  this:save()
end

function on_update(this, elapsedTime)
  this:orient_on_terrain(0.0, 0.0, 0.0)
end

on_collision = boat.on_collision
