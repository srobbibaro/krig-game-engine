local enemy_ship = require 'scripts/enemy_ship'

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  this         = this:load()
  camera       = krig.get_camera():load()
  relative_pos = this.position[1] - camera.position[1]

  if ( relative_pos > 20 ) then return end

  -- Bottom of the curve.
  if ( this.position[2] < 15 ) then
    -- Start moving forward.
    if ( relative_pos < 20 ) then this.velocity = {0.0, 0.0, 0.0} end
    -- Angle a bit up.
    if ( relative_pos < 10 ) then this.velocity = {2.0, 5.0, 0.0} end
    -- Go straight up.
    if ( relative_pos < 5 )  then this.velocity = {10.0, 10.0, 0.0} end

    this:save()
    return
  end

  -- Should be going straight up when we get to here.
  -- Top of the curve.
  if ( this.position[2] >= 15 ) then
    -- Start curving back to the right.
    if this.position[2] > 20 then this.velocity = {18.0, 5.0, 0.0} end
    -- Full retreat.
    if relative_pos > 10 then this.velocity = {20.0, 0.0, 0.0} end
    -- Remove when we get way off the screen.
    if relative_pos > 20 then krig.level.remove_object(this) end
  end

  this:save()
end

on_load      = enemy_ship.on_load
on_collision = enemy_ship.on_collision
