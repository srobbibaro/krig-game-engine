local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)

  camera          = getCamera()
  camera_position = getPosition(camera)

  setPosition(this, camera_position[1] + 20.0, math.random(30), 0.0)
  setVelocity(this, -30.0, 0.0, 0.0)
end

function on_update(this, elapsedTime)
  camera          = getCamera()
  this_position   = getPosition(this)
  camera_position = getPosition(camera)

  if this_position[1] < camera_position[1] - 20 then
    removeObject(this)
  end
end

function on_collision(this, temp)
  asteroid.on_collision(this, temp)

  tempId = getTypeId(temp)

  if tempId == 1 or tempId == 0 then
    removeObject(this)
  end
end
