local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.2)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  plr      = krig.get_player()
  camera   = krig.get_camera()
  this_pos = krig.object.get_position(this)
  plr_pos  = krig.object.get_position(plr)
  in_view  = krig.object.get_in_view(this)
  cam_pos  = krig.object.get_position(camera)
  cam_vel  = krig.object.get_velocity(camera)

  if state == 0 then
    if this_pos[1] < cam_pos[1] - 10.0 then
      krig.object.set_velocity(this, cam_vel[1], 10.0, 0.0)
      state = 1
    end
  elseif state == 1 then
    if this_pos[2] > cam_pos[2] - 9.0 then
      krig.object.set_velocity(this, cam_vel[1] * 4.0, 2.0, 0.0)
      state = 2
    end
  elseif state == 2 then
    if this_pos[1] > cam_pos[1] + 10.0 then
      krig.object.set_velocity(this, cam_vel[1], 10.0, 0.0)
      state = 3
    end
  elseif state == 3 then
    if this_pos[2] > cam_pos[2] + 9.0 then
      krig.object.set_velocity(this, -cam_vel[1] * 3.0, 2.0, 0.0)
      state = 4
    end
  elseif state == 4 then
    if in_view == 0 then krig.level.remove_object(this) end
  end

  if plr_pos[2] < (this_pos[2] + .5) and
     plr_pos[2] > (this_pos[2] - .5) and
     in_view == 1 then
    shooting_object.attemptShot(this, krig.object.get_bounding_sphere_radius(this))
  end
end

on_collision = enemy_ship.on_collision
