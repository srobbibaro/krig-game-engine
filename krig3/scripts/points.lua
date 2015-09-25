local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)

  camera = krig.get_camera():load()

  this.collision_detection_enabled = false
  this.scale                       = {0.25, 0.25, 0.0}
  this.velocity                    = {camera.velocity[1], 2.0, camera.velocity[3]}
  this.fade_rate                   = -0.25
  this.color                       = {1.0, 1.0, 1.0, 1.0}
  this.width                       = 5
  this:save()
end

function on_update(this, elapsedTime)
  if this:load().color[4] <= 0.0 then
    krig.level.remove_object(this)
  end
end
