local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)

  camera = krig.get_camera():load()

  this.collision_detection_enabled = false
  this.scale                       = {0.0051, 0.005, 0.0}
  this.velocity                    = {camera.velocity[1], 2.0, camera.velocity[3]}
  this.fade_rate                   = -0.25
  this.color                       = {0.8, 0.8, 0.8, 0.2}
  this.width                       = 3
  this.always_lit                  = true
  this:save()
end

function on_update(this, elapsedTime)
  if this:load().color[4] <= 0.0 then
    krig.level.remove_object(this)
  end
end
