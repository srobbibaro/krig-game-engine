local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)

  camera = krig.get_camera():load()

  this.position = {camera.position[1] + 20.0, math.random(30), 0.0}
  this.velocity = {-30.0, 0.0, 0.0}
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()

  if this.position[1] < camera.position[1] - 20 then
    krig.level.remove_object(this)
  end
end

function on_collision(this, temp)
  asteroid.on_collision(this, temp)

  temp_id = temp:load().type_id

  if temp_id == 1 or temp_id == 0 then
    krig.level.remove_object(this)
  end
end
