local enemy_ship = require 'scripts/enemy_ship'

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  this_position = getPosition(this)
  camera_pos    = getPosition( getCamera() )
  relative_pos  = this_position[1] - camera_pos[1]
  if ( relative_pos > 20 ) then return end

  -- Bottom of the curve.
  if ( this_position[2] < 15 ) then
    -- Start moving forward.
    if ( relative_pos < 20 ) then setVelocity(this, 0.0, 0.0, 0.0) end
    -- Angle a bit up.
    if ( relative_pos < 10 ) then setVelocity(this, 2.0, 5.0, 0.0) end
    -- Go straight up.
    if ( relative_pos < 5 )  then setVelocity(this, 10.0, 10.0, 0.0) end
    return
  end

  -- Should be going straight up when we get to here.
  -- Top of the curve.
  if ( this_position[2] >= 15 ) then
    -- Start curving back to the right.
    if this_position[2] > 20 then setVelocity(this, 18.0, 5.0, 0.0) end
    -- Full retreat.
    if relative_pos > 10 then setVelocity(this, 20.0, 0.0, 0.0) end
    -- Remove when we get way off the screen.
    if relative_pos > 20 then removeObject(this) end
  end
end

on_load      = enemy_ship.on_load
on_collision = enemy_ship.on_collision
