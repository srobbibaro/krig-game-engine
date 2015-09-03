local enemy           = require 'scripts/enemy'
local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state       = 0
enemy.score = 2000
enemy.life  = 40
life        = enemy.life

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "Boss.mdl")
  krig.object.set_scale(this, 4.0, 4.0, 4.0)
  krig.object.set_rotation(this, 0.0, -1.5708, 0.0)

  krig.object.set_velocity(this, -5.0, 0.0, 0.0)
  krig.object.set_type_id(this, 1)
  krig.object.disable_collision_detection(this)

  shooting_object.setupShots(this, './scripts/boss_shot.lua', 0.85)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  camera = krig.get_camera()
  cam_pos = krig.object.get_position(camera)
  this_pos = krig.object.get_position(this)

  if state == 0 then
    if this_pos[1] <= (cam_pos[1] + 11.0) then
      state = 1
      krig.object.set_velocity(this, 0.0, -10.0, 0.0)
      krig.object.enable_collision_detection(this)
    end
    return
  elseif state == 1 then
    if this_pos[2] < (cam_pos[2] - 5.0) then
      state = 2
      krig.object.set_velocity(this, 0.0, 10.0, 0.0)
    end
  elseif state == 2 then
    if this_pos[2] > (cam_pos[2] + 5.0) then
      state = 1
      krig.object.set_velocity(this, 0.0, -10.0, 0.0)
    end
  end

  plr = krig.get_player()
  plr_pos = krig.object.get_position(plr)
  in_view = krig.object.get_in_view(this)

  if in_view == 1 then
    radius = krig.object.get_bounding_sphere_radius(this) - 1.25
    shooting_object.attemptShots(this, 2, {radius, radius}, {radius, -radius})
  end
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
