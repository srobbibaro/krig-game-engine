local asteroid    = require 'scripts/asteroid'
local game_object = require 'scripts/game_object'

-- Configuration
local state       = 0
local final_x_vel = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("redrock.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
  this.scale    = {4.0, 4.0, 4.0}
  this.type_id  = 4
  this.collision_detection_enabled = false
  game_object.on_load(this, options)
  this:save()

  final_x_vel = options.final_x_vel
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()

  -- Don't start moving until we're close to it.
  if state == 0 then
     if camera.position[1] < this.position[1]-30.0 or
        camera.position[1] > this.position[1]+30.0 then
        return
      end
      state = 1
  end

  -- Set seed for random positions and velocities.
  math.randomseed( this.position[1] )

  -- Stupid line for Windows... Throws out the first (faulty) random value.
  math.random(40);

  -- Setup our rotation.
  this.rotation_velocity = {math.random(0,3), math.random(0,3), math.random(0,3)}

  if state == 2 then
    asteroid.is_out_of_view(this, this.position, camera.position)
    return
  end

  angle_height = 40  -- Change upward direction when we get here.
  fall_height  = 90  -- Move the rock over to the player when we get here

  x_vel = 0.0
  if this.position[2] >= angle_height then
    x_vel = final_x_vel
  end
  this.velocity = {x_vel, 15.0, 0.0}

  if this.position[2] >= fall_height then
    state = 2
    math.randomseed( camera.position[1] )
    x_pos = camera.position[1] + math.random(0,25)
    this.position = {x_pos, 30.0, 7.5}
    this.velocity = {math.random(0,15), -20.0, 0.0}
    this.scale    = {3.0, 3.0, 3.0}
    this.collision_detection_enabled = true
  end
  this:save()
end

on_collision = asteroid.on_collision
