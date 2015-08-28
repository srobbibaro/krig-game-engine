local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state    = 0
prev_pos = nil

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.2)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  plr      = getPlayer()
  camera   = getCamera()
  this_pos = getPosition(this)
  plr_pos  = getPosition(plr)
  in_view  = getInView(this)
  cam_pos  = getPosition(camera)
  cam_vel  = getVelocity(camera)

  if state == 0 then
    if this_pos[1] < cam_pos[1] + 15.0 then
      setVelocity(this, -cam_vel[1], 0.0, 0.0)
      prev_pos = this_pos
      state = 1
    end
  elseif state == 1 then
    if this_pos[1] < prev_pos[1] - 7.0 then
      setVelocity(this, cam_vel[1], -5.0, 0.0)
      prev_pos = this_pos
      state = 2
    end
  elseif state == 2 then
    if this_pos[2] < prev_pos[2] - 8.0 then
      setVelocity(this, 2 * cam_vel[1], 0.0, 0.0)
      prev_pos = this_pos
      state = 3
    end
  elseif state == 3 then
    if this_pos[1] > prev_pos[1] + 10.0 then
      setVelocity(this, cam_vel[1], 5.0, 0.0)
      prev_pos = this_pos
      state = 4
    end
  elseif state == 4 then
    if this_pos[2] > prev_pos[2] + 8.0 then
      setVelocity(this, -cam_vel[1], 0.0, 0.0)
      prev_pos = this_pos
      state = 1
    end
  end

  if in_view == 0 and state ~= 0 then removeObject(this) end

  if plr_pos[2] < (this_pos[2] + .5) and
     plr_pos[2] > (this_pos[2] - .5) and
     in_view == 1 then
    shooting_object.attemptShot(this, getBoundingSphereRadius(this))
  end
end

on_collision = enemy_ship.on_collision
