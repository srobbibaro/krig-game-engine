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
  this.set_model(this, "snowboss.mdl")
  this.rotation = krig.rotation.from_euler({0.0, 1.57, 0.0})
  this.scale    = {4.0, 4.0, 4.0}
  this.type_id  = 1
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  player = krig.get_player():load()
  camera = krig.get_camera():load()

  x_vel       = this.velocity[1]
  x_move_time = x_move_time - elapsedTime

  if x_vel > 0.0 and this.position[1] >= camera.position[1] - 12.0 then
    x_vel = 0.0
  elseif x_vel < 0.0 and this.position[1] <= camera.position[1] - 17.0 then
    x_vel = 0.0
    x_move_time = 0.5
  elseif x_vel == 0.0 and this.position[1] <= camera.position[1] - 17.0 and x_move_time <= 0.0 then
    x_vel = 6.0
  end

  if this.position[2] + 1.0  < player.position[2] then
    this.velocity = {x_vel, 10.0, 0.0}
  elseif this.position[2]  - 1.0 > player.position[2] then
    this.velocity = {x_vel, -10.0, 0.0}
  else
    this.velocity = {x_vel, 0.0, 0.0}
  end
  this:save()
end

function on_collision(this, temp)
  this = this:load()
  enemy.on_collision(this, temp)
  this.velocity[1] = this.velocity[1] - 16.00
  this:save()
  life = enemy.life
end
