local enemy = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  setRotation(this, 0.19625, -2.7475, 0.0)
end

function on_update(this, elapsedTime)
  this_pos = getPosition(this)
  camera   = getCamera()
  cam_pos  = getPosition(camera)
  tx       = cam_pos[1] + 70.0

  if tx >= this_pos[1] and state == 0 then
    setSpeed(this, 20.0, 0.0, 0.0)
    this_dir = getDirection(this)
    setRotationVelocityAxis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
    state = 1
  end
end

function on_collision(this, temp)
  this_id = getTypeId(this)
  temp_id = getTypeId(temp)

  if temp_id == 100 then
    this_pos       = getPosition(this)
    this_radius    = getBoundingSphereRadius(this)
    terrain_height = terrain_getHeight(this_pos[1], this_pos[3])

    if (this_pos[2] - this_radius) <= terrain_height then
      playSound(this, "explosion.wav")

      enemy.create_explosion(this)
      removeObject(this)
    end
  else
    enemy.on_collision(this, temp)
  end
end
