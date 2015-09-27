local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
local state = 0

function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.2)
end

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  player = krig.get_player():load()
  camera = krig.get_camera():load()

  if state == 0 then
    if this.position[1] < camera.position[1] - 10.0 then
      this.velocity = {camera.velocity[1], -10.0, 0.0}
      this:save()
      state = 1
    end
  elseif state == 1 then
    if this.position[2] < camera.position[2] + 9.0 then
      this.velocity = {camera.velocity[1] * 4.0, -2.0, 0.0}
      this:save()
      state = 2
    end
  elseif state == 2 then
    if this.position[1] > camera.position[1] + 10.0 then
      this.velocity = {camera.velocity[1], -10.0, 0.0}
      this:save()
      state = 3
    end
  elseif state == 3 then
    if this.position[2] < camera.position[2] - 9.0 then
      this.velocity = {-camera.velocity[1] * 3.0, -2.0, 0.0}
      this:save()
      state = 4
    end
  elseif state == 4 then
    if not this.in_view then krig.level.remove_object(this) end
  end

  if player.position[2] < (this.position[2] + .5) and
     player.position[2] > (this.position[2] - .5) and
     this.in_view then
    shooting_object.attemptShot(this, this.bounding_sphere_radius)
  end
end

on_collision = enemy_ship.on_collision
