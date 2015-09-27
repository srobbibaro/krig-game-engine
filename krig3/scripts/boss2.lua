local enemy       = require 'scripts/enemy'
local game_object = require 'scripts/game_object'

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
  local camera = krig.get_camera():load()
  x_min = camera.position[1] - math.random(20)
  x_max = camera.position[1] + math.random(20)
  y_min = camera.position[2] - math.random(20)
  y_max = camera.position[2] + math.random(20)
end

function calc_speed()
  local speed = 50.0 - math.random(20)

  if math.random(2) == 1 then
    speed = speed * -1
  end

  return speed
end

-- Overridden Engine Callbacks
function on_load(this, options)
  math.randomseed(os.time())
  set_window()

  game_object.on_load(this, options)

  this:set_model("snowboss.mdl")
  this.rotation = krig.rotation.from_euler({0.0, -1.57, 0.0})
  this.scale    = {4.0, 4.0, 4.0}

  this.velocity = {-calc_speed(), 0.0, 0.0}
  this.type_id  = 1
  this:save()
end

function on_update(this, elapsedTime)
  this = this:load()

  if this.velocity[1] > 0.0 then
    if this.position[1] > x_max then
      this.velocity = {0.0, calc_speed(), 0.0}
      this:save()
    end
  elseif this.velocity[1] < 0.0 then
    if this.position[1] < x_min then
      this.velocity = {0.0, calc_speed(), 0.0}
      this:save()
    end
  elseif this.velocity[2] > 0.0 then
    if this.position[2] > y_max then
      speed = calc_speed()
      this.velocity = {calc_speed(), 0.0, 0.0}
      this:save()
    end
  elseif this.velocity[2] < 0.0 then
    if this.position[2] < y_min then
      this.velocity = {calc_speed(), 0.0, 0.0}
      this:save()
      set_window()
    end
  end
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
