local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)
  this:save()
  this:set_texture("cloud.png")
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()
  this.velocity = camera.velocity
  this:save()
end
