local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)
  setScale(this, 5.0, 5.0, 5.0)
end

function on_update(this, elapsedTime)
  camera          = getCamera()
  this_position   = getPosition(this)
  camera_position = getPosition(camera)

  asteroid.is_out_of_view(this, this_position, camera_position)
end
