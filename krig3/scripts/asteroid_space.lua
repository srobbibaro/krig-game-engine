local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)
  this.scale = {5.0, 5.0, 5.0}
  this:save()
end

function on_update(this, elapsedTime)
  this            = this:load()
  camera          = krig.get_camera():load()

  asteroid.is_out_of_view(this, this.position, camera.position)
end
