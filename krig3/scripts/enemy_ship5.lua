local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state = 0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  setModel(this, "Enemy.mdl")
  setRotation(this, 0.19625, -2.7475, 0.0)
end

function on_update(this, elapsedTime)
  this_pos = getPosition(this)
  camera   = getCamera()
  cam_pos  = getPosition(camera)
  tx       = cam_pos[1] + 70.0

  if tx >= this_pos[1] and state == 0 then
    setSpeed(this, 10.0, 0.0, 0.0)
    this_dir = getDirection(this)
    setRotationVelocityAxis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
    state = 1
  end
end

on_collision = enemy_ship.on_collision
