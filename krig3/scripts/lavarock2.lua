local asteroid = require 'scripts/asteroid'

-- Configuration
local falling = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("redrock.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
  this.scale    = {4.0, 4.0, 4.0}
  this.type_id  = 4
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()

  -- Don't start moving until we're close to it.
  if camera.position[1] < this.position[1]-30.0 or
     camera.position[1] > this.position[1]+30.0 then
    return
  end

  -- Set seed for random positions and velocities.
  math.randomseed( this.position[1] )

  -- Stupid line for Windows... Throws out the first (faulty) random value.
  math.random(40);

  -- Setup our rotation.
  this.rotation_velocity = {math.random(0,3), math.random(0,3), math.random(0,3)}

  if falling == 1 then
    this.velocity = {0.0, -20.0, 0.0}
    this:save()
    return
  end

  angle_height = 40  -- Change upward direction when we get here.
  fall_height  = 90  -- Move the rock over to the player when we get here

  x_vel = 0.0
  if this.position[2] >= angle_height then
    x_vel = -5.0
  end
  this.velocity = {x_vel, 15.0, 0.0}

  if this.position[2] >= fall_height then
    falling = 1
    math.randomseed( camera.position[1] )
    x_pos = camera.position[1] + math.random(-5,20)
    this.position = {x_pos, 30.0, 7.5}
    this.scale    = {3.0, 3.0, 3.0}
  end
  this:save()
end

on_collision = asteroid.on_collision
