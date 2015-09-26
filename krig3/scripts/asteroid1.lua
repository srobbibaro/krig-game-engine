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
  camera = krig.get_camera():load()
  this   = this:load()

  if active_offset + camera.position[1] > this.position[1] then
    this.velocity = {math.random(0,3), -math.random(5, 7), 0.0}
    this.rotation_velocity = {math.random(0, 3), math.random(0, 3), math.random(0, 3)}
    this:save()
  end

  asteroid.is_out_of_view(this, this.position, camera.position)
end

on_collision = asteroid.on_collision
