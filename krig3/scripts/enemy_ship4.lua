local enemy = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  krig.object.set_rotation(this, 0.19625, -2.7475, 0.0)
end

function on_update(this, elapsedTime)
  this_pos = krig.object.get_position(this)
  camera   = krig.get_camera()
  cam_pos  = krig.object.get_position(camera)
  tx       = cam_pos[1] + 70.0

  if tx >= this_pos[1] and state == 0 then
    krig.object.set_speed(this, 20.0, 0.0, 0.0)
    this_dir = krig.object.get_direction(this)
    krig.object.set_rotation_velocity_axis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
    state = 1
  end
end

function on_collision(this, temp)
  this_id = krig.object.get_type_id(this)
  temp_id = krig.object.get_type_id(temp)

  if temp_id == 100 then
    this_pos       = krig.object.get_position(this)
    this_radius    = krig.object.get_bounding_sphere_radius(this)
    terrain_height = krig.terrain.get_height(this_pos[1], this_pos[3])

    if (this_pos[2] - this_radius) <= terrain_height then
      krig.play_sound(this, "explosion.wav")

      enemy.create_explosion(this)
      krig.level.remove_object(this)
    end
  else
    enemy.on_collision(this, temp)
  end
end
