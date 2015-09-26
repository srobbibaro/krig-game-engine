local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state    = 0
prev_pos = nil

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.2)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  player = krig.get_player():load()
  camera = krig.get_camera():load()

  if state == 0 then
    if this.position[1] < camera.position[1] + 15.0 then
      this.velocity = {-camera.velocity[1], 0.0, 0.0}
      prev_pos      = krig.vector.copy(this.position)
      state         = 1
      this:save()
    end
  elseif state == 1 then
    if this.position[1] < prev_pos[1] - 7.0 then
      this.velocity = {camera.velocity[1], -5.0, 0.0}
      prev_pos      = krig.vector.copy(this.position)
      state         = 2
      this:save()
    end
  elseif state == 2 then
    if this.position[2] < prev_pos[2] - 8.0 then
      this.velocity = {2 * camera.velocity[1], 0.0, 0.0}
      prev_pos      = krig.vector.copy(this.position)
      state         = 3
      this:save()
    end
  elseif state == 3 then
    if this.position[1] > prev_pos[1] + 10.0 then
      this.velocity = {camera.velocity[1], 5.0, 0.0}
      prev_pos      = krig.vector.copy(this.position)
      state         = 4
      this:save()
    end
  elseif state == 4 then
    if this.position[2] > prev_pos[2] + 8.0 then
      this.velocity = {-camera.velocity[1], 0.0, 0.0}
      prev_pos      = krig.vector.copy(this.position)
      state         = 1
      this:save()
    end
  end

  if not this.in_view and state ~= 0 then krig.level.remove_object(this) end

  if player.position[2] < (this.position[2] + .5) and
     player.position[2] > (this.position[2] - .5) and
     this.in_view then
    shooting_object.attemptShot(this, this.bounding_sphere_radius)
  end
end

on_collision = enemy_ship.on_collision
