local asteroid = require 'scripts/asteroid'

-- Configuration
local falling = 0

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "redrock.mdl")
  krig.object.set_scale(this, 4.0, 4.0, 4.0)
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)

  krig.object.set_type_id(this, 4)
end

function on_update(this, elapsedTime)
  camera          = krig.get_camera()
  this_position   = krig.object.get_position(this)
  camera_position = krig.object.get_position(camera)

  -- Don't start moving until we're close to it.
  if camera_position[1] < this_position[1]-30.0 or
     camera_position[1] > this_position[1]+30.0 then
    return
  end

  -- Set seed for random positions and velocities.
  math.randomseed( this_position[1] )

  -- Stupid line for Windows... Throws out the first (faulty) random value.
  math.random(40);

  krig.object.set_rotation_velocity(this,
  math.random(0,3), math.random(0,3), math.random(0,3))

  if falling == 1 then
    krig.object.set_velocity(this, 0.0, -20.0, 0.0)
    return
  end

  angle_height = 40  -- Change upward direction when we get here.
  fall_height  = 90  -- Move the rock over to the player when we get here

  x_vel = 0.0
  if this_position[2] >= angle_height then
    x_vel = -5.0
  end
  krig.object.set_velocity(this, x_vel, 15.0, 0.0)

  if this_position[2] >= fall_height then
    falling = 1
    math.randomseed( camera_position[1] )
    x_pos = camera_position[1] + math.random(-5,20)
    krig.object.set_position(this, x_pos, 30.0, 7.5)
    krig.object.set_scale(this, 3.0, 3.0, 3.0)
  end
end

on_collision = asteroid.on_collision
