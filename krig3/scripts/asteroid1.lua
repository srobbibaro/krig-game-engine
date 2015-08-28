dofile('./scripts/base_enemy.lua')

-- Configuration
score = 500

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Asteroid.mdl")

  setScale(this, 2.0, 2.0, 2.0)
  setRotation(this, 0.0, 0.0, 0.0)

  life = 3
  setTypeId(this, 4)

  -- This value is used to determine how "far" the camera should be from
  -- the asteroid before it is activated. Doing it this way provides a little
  -- variance.
  active_offset = math.random(0, 40)
end

function on_update(this, elapsedTime)
  camera          = getCamera()
  camera_position = getPosition(camera)

  this_position   = getPosition(this)

  if active_offset + camera_position[1] > this_position[1] then
    setVelocity(this, math.random(0,3), -math.random(5, 7), 0.0)
    setRotationVelocity(
      this,
      math.random(0, 3), math.random(0, 3), math.random(0, 3)
    )
  end

  asteroid.is_out_of_view(this, this_position, camera_position)
end

on_collision = asteroid.on_collision
