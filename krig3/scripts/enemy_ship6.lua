local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
local state      = 0
local delay      = 0.0
local interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  this:set_model("Enemy.mdl")
  this.rotation = krig.rotation.from_euler({0.19625, -2.7475, 0.0})
  this:save()
end

function on_update(this, elapsedTime)
  this   = this:load()
  camera = krig.get_camera():load()
  tx     = camera.position[1] + 70.0

  if state == 0 then
    if tx >= this.position[1] then
      this.speed             = {10.0, 0.0, 0.0}
      normalized_direction   = krig.vector.normalize(krig.vector.copy(this.direction))
      this.rotation_velocity = krig.vector.scale(normalized_direction, 20.0)
      this:save()
      state = 1
      delay = 5.0
    end
  elseif state == 1 then
    delay = delay - elapsedTime
    if delay <= 0 then
      this.speed = {5.0, 0.0, 0.0}
      this:save()
      state = 2
      delay = 0.0
    end
  elseif state == 2 then
    this:setup_interpolation(this.rotation, 0.0, krig.rotation.from_euler({-0.785, -3.925, 0.0}), 0.5)
    this:update_interpolation_value(0.0)
    this.interpolation_enabled = true

    this.speed             = {30.0, 0.0, 0.0}
    this.rotation_velocity = {0.0, 0.0, 0.0}
    state                  = 3
    interpTime             = 0.0
    this:save()
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)
    if interpTime >= 0.5 then
      this.interpolation_enabled = false
      this:save()
      state = 4
      delay = 0.0
    end
  end
end

on_collision = enemy_ship.on_collision
