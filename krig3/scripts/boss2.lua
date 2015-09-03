local enemy      = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local x_min = 0.0
local x_max = 0.0
local y_min = 0.0
local y_max = 0.0

enemy.score = 5000
enemy.life  = 40
life        = enemy.life

-- Helper Methods
function set_window()
  local camera = krig.get_camera()
  local camera_position = krig.object.get_position(camera)
  x_min = camera_position[1] - math.random(20)
  x_max = camera_position[1] + math.random(20)
  y_min = camera_position[2] - math.random(20)
  y_max = camera_position[2] + math.random(20)
end

function calc_speed()
  local speed = 50.0 - math.random(20)

  if math.random(2) == 1 then
    speed = speed * -1
  end

  return speed
end

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "snowboss.mdl")
  krig.object.set_rotation(this, 0.0, -1.57, 0.0)
  krig.object.set_scale(this, 4.0, 4.0, 4.0)

  math.randomseed(os.time())
  set_window()
  speed = calc_speed()
  krig.object.set_velocity(this, -speed, 0.0, 0.0)

  krig.object.set_type_id(this, 1)
end

function on_update(this, elapsedTime)
  this_position = krig.object.get_position(this)
  this_velocity = krig.object.get_velocity(this)

  if this_velocity[1] > 0.0 then
    if this_position[1] > x_max then
      speed = calc_speed()
      krig.object.set_velocity(this, 0.0, speed, 0.0)
    end
  elseif this_velocity[1] < 0.0 then
    if this_position[1] < x_min then
      speed = calc_speed()
      krig.object.set_velocity(this, 0.0, speed, 0.0)
    end
  elseif this_velocity[2] > 0.0 then
    if this_position[2] > y_max then
      speed = calc_speed()
      krig.object.set_velocity(this, speed, 0.0, 0.0)
    end
  elseif this_velocity[2] < 0.0 then
    if this_position[2] < y_min then
      speed = calc_speed()
      krig.object.set_velocity(this, speed, 0.0, 0.0)
      set_window()
    end
  end
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
