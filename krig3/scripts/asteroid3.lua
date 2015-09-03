local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)

  camera          = krig.get_camera()
  camera_position = krig.object.get_position(camera)

  krig.object.set_position(this, camera_position[1] + 20.0, math.random(30), 0.0)
  krig.object.set_velocity(this, -30.0, 0.0, 0.0)
end

function on_update(this, elapsedTime)
  camera          = krig.get_camera()
  this_position   = krig.object.get_position(this)
  camera_position = krig.object.get_position(camera)

  if this_position[1] < camera_position[1] - 20 then
    krig.level.remove_object(this)
  end
end

function on_collision(this, temp)
  asteroid.on_collision(this, temp)

  tempId = krig.object.get_type_id(temp)

  if tempId == 1 or tempId == 0 then
    krig.level.remove_object(this)
  end
end
