local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state       = 1
orig_height = 0.0
totalTime   = 1.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.5)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  player = krig.get_player():load()

  if state == 1 then
    if player.position[1] > this.position[1] - 25.0 then
      this.velocity = {0.0, 5.0, 0.0}
      orig_height   = this.position[2]
      state         = 2
      this:save()
    end
  elseif state == 2 then
    if this.position[2] >= orig_height + 3.0 then
      this.velocity = {0.0, 0.0, 0.0}
      this.speed    = {40.0, 0.0, 0.0}
      state         = 3
      this:save()
    end
  elseif state == 3 then
    totalTime = totalTime + elapsedTime

    if totalTime > 2.0 then state = 4 end
    this_dir      = {0.0, 0.0, 1.0}
    dir_to_player = krig.vector.normalize({
      this.position[1] - player.position[1] - 2.0,
      this.position[2] - player.position[2],
      this.position[3] - player.position[3]
    })

    rot_axis      = krig.vector.cross_product(dir_to_player, this_dir)
    rot_angle     = -krig.vector.dot_product(dir_to_player, this_dir)
    this.rotation = krig.rotation.from_axis(rot_axis, rot_angle)

    this:save()
  end
end

on_collision = enemy_ship.on_collision
