local enemy      = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
x_min = 0.0
x_max = 0.0
y_min = 0.0
y_max = 0.0

x_move_time = 0.0
enemy.score = 5000
enemy.life  = 40
life        = enemy.life

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "snowboss.mdl")
  setRotation(this, 0.0, 1.57, 0.0)
  setScale(this, 4.0, 4.0, 4.0)

  setTypeId(this, 1)
end

function on_update(this, elapsedTime)
  this_pos      = getPosition(this)
  this_velocity = getVelocity(this)
  player        = getPlayer()
  plr_pos       = getPosition(player)
  camera        = getCamera()
  cam_pos       = getPosition(camera)

  x_vel       = this_velocity[1]
  x_move_time = x_move_time - elapsedTime

  if x_vel > 0.0 and this_pos[1] >= cam_pos[1] - 12.0 then
    x_vel = 0.0
  elseif x_vel < 0.0 and this_pos[1] <= cam_pos[1] - 17.0 then
    x_vel = 0.0
    x_move_time = 0.5
  elseif x_vel == 0.0 and this_pos[1] <= cam_pos[1] - 17.0 and x_move_time <= 0.0 then
    x_vel = 6.0
  end

  if this_pos[2] + 1.0  < plr_pos[2] then
    setVelocity(this, x_vel, 10.0, 0.0)
  elseif this_pos[2]  - 1.0 > plr_pos[2] then
    setVelocity(this, x_vel, -10.0, 0.0)
  else
    setVelocity(this, x_vel, 0.0, 0.0)
  end
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  addVelocity(this, -16.0, 0.0, 0.0)
  life = enemy.life
end
