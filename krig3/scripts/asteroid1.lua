local asteroid = require 'scripts/asteroid'

-- Overridden Engine Callbacks
function on_load(this, options)
  asteroid.on_load(this, options)

  -- This value is used to determine how "far" the camera should be from
  -- the asteroid before it is activated. Doing it this way provides a little
  -- variance.
  active_offset = math.random(0, 40)
end

function on_update(this, elapsedTime)
  camera          = krig.get_camera()
  camera_position = krig.object.get_position(camera)

  this_position   = krig.object.get_position(this)

  if active_offset + camera_position[1] > this_position[1] then
    krig.object.set_velocity(this, math.random(0,3), -math.random(5, 7), 0.0)
    krig.object.set_rotation_velocity(
      this,
      math.random(0, 3), math.random(0, 3), math.random(0, 3)
    )
  end

  asteroid.is_out_of_view(this, this_position, camera_position)
end

on_collision = asteroid.on_collision
