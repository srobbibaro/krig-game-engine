local enemy = require 'scripts/enemy'

-- Configuration
enemy.life = 10

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "CannonBall.mdl")
  krig.object.set_scale(this, 2.0, 2.0, 2.0)
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)
  -- krig.object.set_type_id(this, 4)
end

function on_update(this, elapsedTime)
  camera          = krig.get_camera()
  this_position   = krig.object.get_position(this)
  camera_position = krig.object.get_position(camera)

  local offset = this_position[1] - 20
  if camera_position[1] >= offset then
    local y_vel = 2.0
    local z_vel = 30.0

    if this_position[2] <= 12.5 then
      y_vel = 15.0
      z_vel = 12.0
    elseif this_position[2] <= 15.0 then
      y_vel = 12.0
      z_vel = 15.0
    elseif this_position[2] <= 17.5 then
      y_vel = 10.0
      z_vel = 17.5
    elseif this_position[2] <= 19.0 then
      y_vel = 8.0
      z_vel = 20.0
    elseif this_position[2] <= 20.0 then
      y_vel = 6.0
      z_vel = 25.0
    end

    krig.object.set_velocity(this, 0.0, y_vel, z_vel )
  end
end

on_collision = enemy.on_collision
