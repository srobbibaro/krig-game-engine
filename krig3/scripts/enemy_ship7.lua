local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)

  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.25)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  player = krig.get_player():load()

  if not this.in_view then return end

  if player.position[2] > this.position[2] then
    this.velocity = {2.0, 3.0, 0.0}
    this:save()
  elseif player.position[2] < this.position[2] then
    this.velocity = {2.0, -3.0, 0.0}
    this:save()
  end

  if player.position[2] < (this.position[2] + .5) and
     player.position[2] > (this.position[2] - .5) and
    this.in_view then
    shooting_object.attemptShot(this, this.bounding_sphere_radius)
  end
end

on_collision = enemy_ship.on_collision
