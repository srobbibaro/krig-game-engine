local scenery = require 'scripts/scenery'

-- Configuration
local shake = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("volcano.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 1.0, 0.0})
  this.scale    = {20.0, 20.0, 20.0}
  scenery.on_load(this, options)
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()

  -- Don't start shaking until we're close to it.
  if camera.position[1] < this.position[1]-30.0 or
     camera.position[1] > this.position[1]+30.0 then
    return
  end

  -- We're in range, shake it baby.
  if shake == 0 then
    this.scale = {21.0, 21.0, 21.0}
    shake = 1
  else
    this.scale = {20.0, 20.0, 20.0}
    shake = 0
  end

  this:save()
end
