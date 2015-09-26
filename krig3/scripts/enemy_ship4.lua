local enemy = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  this.rotation = krig.rotation.from_euler({0.19625, -2.7475, 0.0})
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()
  tx     = camera.position[1] + 70.0

  if tx >= this.position[1] and state == 0 then
    this.speed             = {20.0, 0.0, 0.0}
    normalized_direction   = krig.vector.normalize(krig.vector.copy(this.direction))
    this.rotation_velocity = krig.vector.scale(normalized_direction, 20.0)
    this:save()
    state = 1
  end
end

function on_collision(this, temp)
  this    = this:load()
  this_id = this.type_id
  temp_id = temp:load().type_id

  if temp_id == 100 then
    this_pos       = this.position
    this_radius    = this.bounding_sphere_radius
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
