local scenery = require 'scripts/scenery'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("FishBoss.mdl")
  this.scale    = {16.0, 16.0, 16.0}
  this.rotation = krig.rotation.from_euler({0.0, 1.5708, 1.2})
  this.type_id  = 3

  scenery.on_load(this, options)

  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()

  if state == 0 then
    if camera.position[1] - 50.0 > this.position[1] then
      this_rotation = krig.rotation.to_euler(this.rotation)
      this_rotation[3] = -2.0
      this:setup_interpolation(
        this.rotation, 0.0,
        krig.rotation.from_euler(this_rotation), 4.0
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      this.speed = {40.0, 0.0, 0.0}
      state      = 1
      interpTime = 0.0

      this:save()
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 4.0 then
      state = 2
    end
  elseif state == 2 then
    if this.position[2] < -10.0 then
      krig.level.remove_object(this)
    end
  end
end
