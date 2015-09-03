local enemy_ship = require 'scripts/enemy_ship'

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  this_position = krig.object.get_position(this)
  camera_pos    = krig.object.get_position( krig.get_camera() )
  relative_pos  = this_position[1] - camera_pos[1]
  if ( relative_pos > 20 ) then return end

  -- Top of the curve.
  if ( this_position[2] >= 15 ) then
    -- Start moving forward.
    if ( relative_pos < 20 ) then krig.object.set_velocity(this, 0.0, 0.0, 0.0) end
    -- Angle a bit down.
    if ( relative_pos < 5 ) then krig.object.set_velocity(this, 2.0, -5.0, 0.0) end
    -- Go straight down.
    if ( relative_pos < 0 ) then krig.object.set_velocity(this, 10.0, -10.0, 0.0) end
    return
  end

  -- Should be going straight down when we get to here.
  -- Bottom of the curve.
  if ( this_position[2] < 15 ) then
    -- Start curving back to the right.
    if this_position[2] < 10 then krig.object.set_velocity(this, 18.0, -5.0, 0.0) end
    -- Full retreat.
    if relative_pos > 5 then krig.object.set_velocity(this, 20.0, 0.0, 0.0) end
    -- Remove when we get way off the screen.
    if relative_pos > 20  then krig.level.remove_object(this) end
  end
end

on_load      = enemy_ship.on_load
on_collision = enemy_ship.on_collision
