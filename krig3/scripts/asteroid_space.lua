local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)
  krig.object.set_scale(this, 5.0, 5.0, 5.0)
end

function on_update(this, elapsedTime)
  camera          = krig.get_camera()
  this_position   = krig.object.get_position(this)
  camera_position = krig.object.get_position(camera)

  asteroid.is_out_of_view(this, this_position, camera_position)
end
